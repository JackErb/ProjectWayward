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

using std::vector;

vector<vector<ChunkType>> GenerateGrid(const GeneratorOptions& opt, LevelData *level, Random *r) {
    vector<vector<ChunkType>> grid;
    for (int x = 0; x < opt.mapWidth; x++) {
        grid.push_back(vector<ChunkType>());
        for (int y = 0; y < opt.mapHeight; y++) {
            grid[x].push_back(ChunkType::Standard);
        }
    }
    
    int path_x = r->rand(0, opt.mapWidth);
    int path_y = 0;
    
    level->start_x = path_x;
    
    int dir = 0;
    
    ChunkType nextPath = Main;
    int drops = 0;
    
    while (path_y < opt.mapHeight) {
        grid[path_x][path_y] = nextPath;
        
        float rf = r->rand();
        if (rf < 0.25 - drops * 0.09) {
            grid[path_x][path_y] = nextPath == MainLand ? MainFall : MainDrop;
            path_y++;
            nextPath = MainLand;
            dir = 0;
            drops++;
        } else {
            if (dir == 0)
                dir = r->rand() < 0.5 ? -1 : 1;
            path_x += dir;
            
            if (path_x < 0 || path_x >= opt.mapWidth) {
                path_x -= dir;
                grid[path_x][path_y] = nextPath == MainLand ? MainFall : MainDrop;
                
                path_y++;
                nextPath = MainLand;
                dir *= -1;
                drops++;
            } else {
                nextPath = Main;
                drops = 0;
            }
        }
    }
    
    for (int y = 0; y < opt.mapHeight; y++) {
        for (int x = 0; x < opt.mapWidth; x++) {
            char c;
            switch (grid[x][y]) {
                case Main:
                    c = '=';
                    break;
                case MainLand:
                    c = '-';
                    break;
                case MainDrop:
                case MainFall:
                    c = 'v';
                    break;
                case Degen:
                case Standard:
                    c = ' ';
                    break;
            }
            printf("%c", c);
        }
        printf("\n");
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
            level->GenerateRandChunk(x, y, grid[x][y], r);
        }
    }
}

LevelData GenerateLevel(const GeneratorOptions& opt) {
    Random r(rand());
    LevelData level;
    vector<vector<ChunkType>> grid = GenerateGrid(opt, &level, &r);
    GenerateLevelDataFromGrid(opt, grid, &level, &r);
    return level;
}

