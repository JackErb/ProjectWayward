//
//  PhysicsEngine.cpp
//  SFML-App
//
//  Created by Jack Erb on 4/25/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "PhysicsEngine.hpp"
#include "../../Generator/LevelData.hpp"
#include "../Entities/StageEntity.hpp"

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
using std::min;
using std::max;

vector<VectorV> get_orthogonals(const PolygonV &p1, int dir1, const PolygonV &p2, int dir2);

pair<bool, VectorV>
    is_separating_axis(const VectorV &axis,
                       const PolygonV &p1, const VectorV &pos1, int dir1,
                       const PolygonV &p2, const VectorV &pos2, int dir2);

bool PhysicsEngine::Intersects(const Entity &e1, const Entity &e2) {
    Rectangle b1 = e1.BoundingBox(), b2 = e2.BoundingBox();
    if (b1.x > b2.x + b2.w || b2.x > b1.x + b1.w) return false;
    if (b1.y > b2.y + b2.h || b2.y > b1.y + b1.h) return false;
    return true;
}

void PhysicsEngine::Update() {
    // Apply velocity to entity position, and update velocity
    // due to physical impulses (traction, gravity, etc.)
    for (Entity *e : entities_) {
        e->Tick();
    }
    
    // Check for collisions
    for (Character *character : characters_) {
        for (Entity *e : entities_) {
            if (character == e) continue;
            for (const PolygonV &p1 : character->polygons) {
                for (const PolygonV &p2 : e->polygons) {
                    auto res = checkCollision(p1, character->Position(), character->Direction(),
                                              p2, e->Position(), e->Direction());
                    if (res.first)
                        character->HandleCollision(*e, res.second);
                }
            }
            
            if (!e->isStatic) {
                checkHitboxCollision(character, e);
            }
        }
    }
}

bool PhysicsEngine::CheckBoundingBoxAgainstPolys(const Entity *e1, const Entity *e2) {
    RectangleV box = e1->BoundingBox();
    PolygonV p1 = {{box.x, box.y}, {box.x + box.w, box.y},
        {box.x + box.w, box.y + box.h}, {box.x, box.y + box.h}};
    
    VectorV pos1 = e1->Position(), pos2 = e2->Position();
    int dir1 = e1->Direction(), dir2 = e2->Direction();
    for (const PolygonV& p2 : e2->polygons) {
        if (checkCollision(p1, pos1, dir1,
                           p2, pos2, dir2).first)
            return true;
    }
    return false;
}

void PhysicsEngine::checkHitboxCollision(Entity *e1, Entity *e2) {
    for (auto it = e1->Hitboxes().begin(); it != e1->Hitboxes().end(); it++) {
        for (const HitboxData &h1 : it->second) {
            if (e1->IgnoreHit(e2->id, h1.id)) {
                continue;
            }
            for (const PolygonV &p2 : e2->polygons) {
                auto res = checkCollision(h1.hitbox, e1->Position(), e1->Direction(),
                                          p2, e2->Position(), e2->Direction());
                if (res.first) {
                    int f = (h1.dmg * fpoat(0,4)).i();
                    bool r = e2->HandleHit(e1, f, h1);
                    if (r) {
                        e1->AddIgnoreHit(e2->id, h1.id);
                        e1->Freeze(f);
                    }
                }
            }
        }
    }
}

void PhysicsEngine::RollbackTick() {
    for (Entity *e : entities_) {
        e->RollbackTick();
    }
}

void PhysicsEngine::Rollback() {
    // TODO: Rollback internal state of PhysicsEngine
    
    
    for (Entity *e : entities_) {
        e->Rollback();
    }
}

pair<bool, VectorV>
  PhysicsEngine::checkCollision(const PolygonV &p1, const VectorV &pos1, int dir1,
                                const PolygonV &p2, const VectorV &pos2, int dir2) {
    // Separating Axis Theorem
    if (p1.size() == 2 && p2.size() == 2) {
        // They are both circles
		fpoat x1 = p1[0].x;
		if (dir1 == -1) x1.sign = !x1.sign;
		fpoat x2 = p2[0].x;
		if (dir2 == -1) x2.sign = !x2.sign;

        VectorV c1 = VectorV(x1, p1[0].y) + pos1;
        VectorV c2 = VectorV(x2, p2[0].y) + pos2;

		fpoat xx = c2.x - c1.x;
		fpoat yy = c2.y - c1.y;
		fpoat dist = xx * xx + yy * yy;
		fpoat rad = p1[1].x + p2[1].x;
        // TODO: Calculate push vec
        if (dist < rad * rad) {
            return {true, {0,0}};
        }
        return {false, {0,0}};
    } else if (p2.size() == 2) {
        // Call with circle in first parameter
        return checkCollision(p2, pos2, dir2, p1, pos1, dir1);
    }
    
    // Get the perpendicular vectors for each side of both polygons
    vector<VectorV> orthogonals = get_orthogonals(p1, dir1, p2, dir2);
    
    // Check if there is a separating axis along each orthogonal
    vector<VectorV> push_vectors;
    for (const auto &vec : orthogonals) {
        auto res = is_separating_axis(unit_vec(vec), p1, pos1, dir1, p2, pos2, dir2);
        if (res.first) {
            // The polygons do not collide
            return make_pair(false, VectorV(0,0));
        } else {
            push_vectors.push_back(res.second);
        }
    }

    VectorV min_pv = push_vectors[0];
    for (int i = 1; i < push_vectors.size(); i++) {
        VectorV pv = push_vectors[i];
        if (dot(pv, pv) < dot(min_pv, min_pv)) min_pv = pv;
    }
    
    // Check that the push vector is pointing the right direction
    VectorV displacement = (geometric_center(p2, dir2) + pos2) -
      (geometric_center(p1, dir1) + pos1);
    if (dot(displacement, min_pv) > 0) {
		min_pv.x.sign = !min_pv.x.sign;
        min_pv.y.sign = !min_pv.y.sign;
    }
    
    return make_pair(true, min_pv);
}

/* Returns the orthogonal vectors of all sides of this polygon. */
vector<VectorV> get_orthogonals(const PolygonV &p1, int dir1, const PolygonV &p2, int dir2) {
    vector<VectorV> res;
    
    int n = p1.size();
    if (n == 2) {
        // p1 is a circle
        
        // The closest point in p2 to p1
		fpoat x1 = p1[0].x;
		if (dir1 == -1) x1.sign = !x1.sign;

        VectorV vec;
		fpoat dist = fpoat::MAX;
        int i = 0;
        for (VectorV v : p2) {
			if (dir2 == -1) v.x.sign = !v.x.sign;

			fpoat xx = v.x - x1;
			fpoat yy = v.y - p1[0].y;
			fpoat d = xx * xx + yy * yy;
            if (d < dist) {
                dist = d;
                vec = v;
            }
            i++;
        }
        res.push_back(VectorV(vec.x, vec.y) - VectorV(x1, p1[0].y));
        res.push_back({-res[0].y,res[0].x});
    } else {
        // Get the edge vectors
        for (int i = 0; i < n; i++) {
            VectorV pp1 = p1[(i+1) % n];
            VectorV pp2 = p1[i];
			if (dir1 == -1) {
				pp1.x.sign = !pp1.x.sign;
				pp2.x.sign = !pp2.x.sign;
			}
            res.push_back(pp1 - pp2);
        }
    }
    
    // Get the edge vectors
    long n2 = p2.size();
    for (int i = 0; i < n2; i++) {
        VectorV pp1 = p2[(i+1) % n];
        VectorV pp2 = p2[i];
		if (dir2 == -1) {
			pp1.x.sign = !pp1.x.sign;
			pp2.x.sign = !pp2.x.sign;
		}
        res.push_back(pp1 - pp2);
    }
    
    // Calculate their orthogonal
    for (int i = 0; i < res.size(); i++) {
        fpoat x = res[i].x, y = res[i].y;
        res[i].x = -y;
        res[i].y = x;
    }
    
    return res;
}

/* Calculate if the given axis is a separating axis for the two polygons.
 * If it is, returns a pair (true, pv) where pv is the push vector
 * If not, returns the pair (false, pv) where pv is (0,0)
 */
pair<bool, VectorV> is_separating_axis(const VectorV &axis,
                                            const PolygonV &p1, const VectorV &pos1, int dir1,
                                            const PolygonV &p2, const VectorV &pos2, int dir2) {
    fpoat min1 = FixedPoint::MAX,
          max1 = FixedPoint::MIN,
          min2 = min1,
          max2 = max1;
        
    if (p1.size() == 2) {
        // p1 is a circle
		fpoat x = p1[0].x;
		if (dir1 == -1) x.sign = !x.sign;

        fpoat proj = dot(axis, VectorV(x, p1[0].y) + pos1);
        min1 = proj - p1[1].x;
        max1 = proj + p1[1].x;
    } else {
        for (const VectorV &vert : p1) {
			fpoat x = vert.x;
			if (dir1 == -1) x.sign = !x.sign;

            fpoat proj = dot(axis, VectorV(x, vert.y) + pos1);
            min1 = fpmin(proj, min1);
            max1 = fpmax(proj, max1);
        }
    }

    for (const VectorV &vert : p2) {
		fpoat x = vert.x;
		if (dir2 == -1) x.sign = !x.sign;

        fpoat proj = dot(axis, VectorV(x, vert.y) + pos2);
        min2 = fpmin(proj, min2);
        max2 = fpmax(proj, max2);
    }
    
    if (max1 >= min2 && max2 >= min1) {
        // Calculate push vector
        fpoat d = fpmin(max2 - min1, max1 - min2);
        VectorV push_vector(axis * (d / dot(axis, axis) + fpoat(1)));
        return make_pair(false, push_vector);
    } else {
        return make_pair(true, VectorV(fpoat(0),fpoat(0)));
    }
}

void PhysicsEngine::SetLevel(LevelData *level) {
    // Generate StageEntity for each chunk in the level
    for (const auto& row : level->chunks) {
        for (const ChunkData chk : row) {
            VectorV pos = {FixedPoint::FromInt(chk.x), FixedPoint::FromInt(chk.y)};
            StageEntity *chunk = new StageEntity(chunkId_++, pos);
            chunk->isStatic = true;
            chunk->polygons = chk.mesh;
            chunk->drawPolygons = true;
            AddStage(chunk);
        }
    }
}
