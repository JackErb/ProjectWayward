//
//  LevelData.cpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/10/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "LevelData.hpp"
#include "Generator.hpp"

#include <string>

void LevelData::GenerateRandChunk(int x, int y, ChunkType typ, Random *r) {
    std::string rep;
    ChunkData *chunk = &chunks[x][y];
    switch (typ) {
        case Main:
            chunk->mesh.push_back({{fpoat(0),fpoat(0)},{fpoat(1600,0),fpoat(0)},
                {fpoat(1600,0),fpoat(400,0)},{fpoat(0),fpoat(400,0)}});
            chunk->mesh.push_back({{fpoat(0),fpoat(1200,0)},{fpoat(1600,0),fpoat(1200,0)},
                {fpoat(1600,0),fpoat(1600,0)},{fpoat(0),fpoat(1600,0)}});
            break;
        case MainDrop:
            chunk->mesh.push_back({{fpoat(0),fpoat(0)},{fpoat(1600,0),fpoat(0)},
                {fpoat(1600,0),fpoat(400,0)},{fpoat(0),fpoat(400,0)}});
            chunk->mesh.push_back({{fpoat(0),fpoat(1200,0)},{fpoat(600,0),fpoat(1200,0)},
                {fpoat(600,0),fpoat(1600,0)},{fpoat(0),fpoat(1600,0)}});
            chunk->mesh.push_back({{fpoat(1000,0),fpoat(1200,0)},{fpoat(1600,0),fpoat(1200,0)},
                {fpoat(1600,0),fpoat(1600,0)},{fpoat(1000,0),fpoat(1600,0)}});
            break;
        case MainFall:
            chunk->mesh.push_back({{fpoat(0),fpoat(0)},{fpoat(600,0),fpoat(0)},
                {fpoat(600,0),fpoat(1600,0)},{fpoat(0),fpoat(1600,0)}});
            chunk->mesh.push_back({{fpoat(1000,0),fpoat(0)},{fpoat(1600,0),fpoat(0)},
                {fpoat(1600,0),fpoat(1600,0)},{fpoat(1000,0),fpoat(1600,0)}});
            break;
        case MainLand:
            chunk->mesh.push_back({{fpoat(0),fpoat(0)},{fpoat(600,0),fpoat(0)},
                {fpoat(600,0),fpoat(400,0)},{fpoat(0),fpoat(400,0)}});
            chunk->mesh.push_back({{fpoat(1000,0),fpoat(0)},{fpoat(1600,0),fpoat(0)},
                {fpoat(1600,0),fpoat(400,0)},{fpoat(1000,0),fpoat(400,0)}});
            chunk->mesh.push_back({{fpoat(0),fpoat(1200,0)},{fpoat(1600,0),fpoat(1200,0)},
                {fpoat(1600,0),fpoat(1600,0)},{fpoat(0),fpoat(1600,0)}});
            break;
        default:
            chunk->mesh.push_back({{fpoat(0),fpoat(0)},{fpoat(1600,0),fpoat(0)},
                {fpoat(1600,0),fpoat(1600,0)},{fpoat(0),fpoat(1600,0)}});
            break;
    }
}

void LevelData::Init(const GeneratorOptions& opt) {
    for (int x = 0; x < opt.mapWidth; x++) {
        chunks.push_back(vector<ChunkData>());
        for (int y = 0; y < opt.mapHeight; y++) {
            chunks[x].push_back(ChunkData());
            chunks[x][y].x = x * 1600;
            chunks[x][y].y = y * 1600;
        }
    }
}
