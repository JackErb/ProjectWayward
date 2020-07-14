//
//  Chunks.cpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/7/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Chunks.hpp"

#include <iostream>
#include <stack>

using std::cerr;
using std::endl;

void ChunkData::GenerateTiles(const std::string& rep) {
    tiles.clear();
    for (int x = 0; x < chunk_w; x++) {
        tiles.push_back(std::vector<TileData>());
        for (int y = 0; y < chunk_h; y++) {
            tiles[x].push_back(TileData());
            tiles[x][y].meshed = false;
            char c = rep[y * chunk_w + x];
            switch (c) {
                case 'o':
                    tiles[x][y].type = Tile_Static;
                    break;
                case ' ':
                    tiles[x][y].type = Tile_Air;
                    break;
                default:
                    cerr << "GENERATOR Undefined char for chunk rep " << c << endl;
            }
        }
    }
}

void ChunkData::GenerateMesh() {
    fpoat chk_x = FixedPoint::FromInt(chunk_x);
    fpoat chk_y = FixedPoint::FromInt(chunk_y);
    
    fpoat tw = FixedPoint::FromInt(tile_w);
    fpoat th = FixedPoint::FromInt(tile_h);
    
    for (int x = 0; x < chunk_w; x++) {
        for (int y = 0; y < chunk_h; y++) {
            // This tile has already been meshed; ignore.
            if (tiles[x][y].meshed) continue;
            
            // This tile does not need to be meshed.
            if (tiles[x][y].type == Tile_Air) continue;
            
            // Exposed if next to an air block or on chunk border.
            bool exposed = x == 0   || tiles[x-1][y].type == Tile_Air
                || x == chunk_w - 1 || tiles[x+1][y].type == Tile_Air
                || y == 0           || tiles[x][y-1].type == Tile_Air
                || y == chunk_h - 1 || tiles[x][y+1].type == Tile_Air;
            
            if (exposed) {
                // This tile needs to be meshed.
                
                // Mesh is rectangle from (rx,ry) to (rx+rw,ry+rh)
                int rx = x, ry = y;
                int rw = 0, rh = 0;
                
                // Expand rectangle in all possible directions
                while (rx + rw + 1 < chunk_w && tiles[rx+rw+1][ry].type == Tile_Static
                       && !tiles[rx+rw+1][ry].meshed) {
                    rw++;
                }
                
                while (rx - 1 >= 0 && tiles[rx-1][ry].type == Tile_Static
                       && !tiles[rx-1][ry].meshed) {
                    rx--;
                }
                
                while (ry + rh + 1 < chunk_h) {
                    bool expand = true;
                    for (int px = rx; px <= rx + rw; px++) {
                        if (tiles[px][ry+rh+1].type != Tile_Static || tiles[px][ry+rh+1].meshed) {
                            expand = false;
                            break;
                        }
                    }
                    
                    if (expand) rh++;
                    else break;
                }
                
                while (ry - 1 >= 0) {
                    bool expand = true;
                    for (int px = rx; px <= rx + rw; px++) {
                        if (tiles[px][ry-1].type != Tile_Static || tiles[px][ry-1].meshed) {
                            expand = false;
                            break;
                        }
                    }
                    
                    if (expand) ry--;
                    else break;
                }
                
                for (int px = rx; px <= rx + rw; px++) {
                    for (int py = ry; py <= ry + rh; py++) {
                        tiles[px][py].meshed = true;
                    }
                }
                
                // Calculate rectangle mesh
                fpoat px = chk_x + FixedPoint::FromInt(rx) * tw;
                fpoat py = chk_y + FixedPoint::FromInt(ry) * th;
                fpoat pw = FixedPoint::FromInt(rw+1) * tw;
                fpoat ph = FixedPoint::FromInt(rh+1) * th;
                mesh.push_back({{px,py},{px+pw,py},{px+pw,py+ph},{px,py+ph}});
            }
        }
    }
}
