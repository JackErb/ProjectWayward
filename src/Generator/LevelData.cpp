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

void LevelData::Init(const GeneratorOptions& opt) {
    for (int x = 0; x < opt.mapWidth; x++) {
        chunks.push_back(vector<ChunkData>());
        for (int y = 0; y < opt.mapHeight; y++) {
            chunks[x].push_back(ChunkData());
            chunks[x][y].chunk_x = x * (opt.chunkWidth * opt.tileWidth) / 2;
            chunks[x][y].chunk_y = y * (opt.chunkHeight * opt.tileHeight) / 2;
            chunks[x][y].chunk_w = opt.chunkWidth;
            chunks[x][y].chunk_h = opt.chunkHeight;
            chunks[x][y].tile_w  = opt.tileWidth;
            chunks[x][y].tile_h  = opt.tileHeight;
        }
    }
}
