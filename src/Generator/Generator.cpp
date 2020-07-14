//
//  Generator.cpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/7/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#include "Generator.hpp"
#include "LevelData.hpp"
#include "Chunks.hpp"

#include "../MathHelper/MathHelper.h"
#include "../MathHelper/Random.h"

#include <vector>
#include <chrono>

using std::vector;

vector<vector<ChunkType>> GenerateGrid(const GeneratorOptions& opt, LevelData *level, Random *r) {
    vector<vector<ChunkType>> grid;
    for (int x = 0; x < opt.mapWidth; x++) {
        grid.push_back(vector<ChunkType>());
        for (int y = 0; y < opt.mapHeight; y++) {
            grid[x].push_back(Chunk_Standard);
        }
    }
    
    int path_x = r->rand(0, opt.mapWidth);
    int path_y = 0;
    
    level->start_x = path_x;
    
    int dir = 0;
    
    ChunkType nextPath = Chunk_Main;
    int drops = 0;
    
    while (path_y < opt.mapHeight) {
        grid[path_x][path_y] = nextPath;
        
        float rf = r->rand();
        if (rf < 0.3 - drops * 0.09) {
            grid[path_x][path_y] = nextPath == Chunk_MainLand ?
                Chunk_MainFall : Chunk_MainDrop;
            path_y++;
            nextPath = Chunk_MainLand;
            dir = 0;
            drops++;
        } else {
            if (dir == 0)
                dir = r->rand() < 0.5 ? -1 : 1;
            path_x += dir;
            
            if (path_x < 1 || path_x >= opt.mapWidth - 1) {
                path_x -= dir;
                grid[path_x][path_y] = nextPath == Chunk_MainLand ?
                    Chunk_MainFall : Chunk_MainDrop;
                
                path_y++;
                nextPath = Chunk_MainLand;
                dir *= -1;
                drops++;
            } else {
                nextPath = Chunk_Main;
                drops = 0;
            }
        }
    }
    
    return grid;
}

void GenerateLevelDataFromGrid(const GeneratorOptions& opt,
                                    vector<vector<ChunkType>> grid,
                                    LevelData *level,
                                    Random *r) {
    level->Init(opt);
    for (int x = 0; x < opt.mapWidth; x++) {
        for (int y = 0; y < opt.mapHeight; y++) {
            ChunkType type = grid[x][y];
            
            auto templates = opt.chunkTemplates->find(type)->second;
            int rand = r->rand(0, templates.size());
            level->chunks[x][y].GenerateTiles(templates[rand].chunk);
            level->chunks[x][y].GenerateMesh();
        }
    }
}

LevelData GenerateLevel(const GeneratorOptions& opt) {
    // Seed random number generator
    Random r(rand());
    
    LevelData level;
    vector<vector<ChunkType>> grid = GenerateGrid(opt, &level, &r);
    GenerateLevelDataFromGrid(opt, grid, &level, &r);
    return level;
}

