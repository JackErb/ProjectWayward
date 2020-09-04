#include "Generator.h"
#include "LevelData.h"
#include "Chunks.h"

#include <vector>
#include <chrono>
#include <iostream>
#include <ww_math.h>
#include <cstdlib>
#include <ctime>

using std::vector;
using std::cout;
using std::endl;

typedef enum Dir {
    Left, Right, Down, Up
} Dir;

vector<vector<ChunkType>> generateGrid(const GeneratorOptions& opt, LevelData *level, Random *r) {
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
        if (rf < 0.2 - drops * 0.1) {
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
            
            if (path_x < 0 || path_x > opt.mapWidth - 1) {
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

    grid[path_x][path_y-1] = Chunk_MainDrop;
    
    return grid;
}

static int DEGEN_PATH_MAX = 5;

bool isFilled(ChunkType type) {
    return type != Chunk_Standard;
}

vector<Dir> getPossibleDirs(vector<vector<ChunkType>> *grid, int x, int y) {
    vector<Dir> res;
    if (x > 0 && !isFilled((*grid)[x-1][y]))
        res.push_back(Left);
    if (x < grid->size() - 1 && !isFilled((*grid)[x+1][y]))
        res.push_back(Right);
    if (y < (*grid)[0].size() - 1 && !isFilled((*grid)[x][y+1]))
        res.push_back(Down);
    if (y > 0 && !isFilled((*grid)[x][y-1]))
        res.push_back(Up);
    return res;
}

void generateDegen(const GeneratorOptions& opt,
                   vector<vector<ChunkType>> *grid,
                   int x,
                   int y,
                   Dir dir,
                   Random *r) {
    
    // Check if initial direction is valid
    vector<Dir> dirs = getPossibleDirs(grid, x, y);
    bool valid = false;
    for (Dir d : dirs) {
        if (d == dir) {
            valid = true;
            break;
        }
    }
    
    if (!valid) return;
    
    for (int i = 0; i < DEGEN_PATH_MAX; i++) {
        switch (dir) {
            case Left:
                x--;
                break;
            case Right:
                x++;
                break;
            case Down:
                y++;
                break;
            case Up:
                y--;
                break;
        }
        
        (*grid)[x][y] = Chunk_Degen;
        
        // Choose next direction
        dirs = getPossibleDirs(grid, x, y);
        if (r->rand() < 0.5f) {
            vector<Dir> copy(dirs);
            dirs.clear();
            while (copy.size() > 0) {
                int idx = r->rand(0, copy.size());
                dirs.push_back(copy[idx]);
                copy.erase(copy.begin() + idx);
            }
        }
        
        bool dirSet = false;
        for (Dir d : dirs) {
            float lim = 0.f;
            switch (d) {
                case Left:
                case Right:
                    lim = .7f;
                    break;
                case Up:
                case Down:
                    lim = .5f;
                    break;
            }
            
            if (r->rand() < lim) {
                dir = d;
                dirSet = true;
                break;
            }
        }
        
        if (!dirSet) break;
    }
}

void generateDegenPaths(const GeneratorOptions& opt,
                        vector<vector<ChunkType>> *grid,
                        LevelData *level,
                        Random *r) {
    // (x,y) is a chunk on the path from top to bottom
    int x = level->start_x, y = 0;
    Dir dir;
    
    while (y < opt.mapHeight) {
        ChunkType chkType = (*grid)[x][y];
        Dir prevDir = dir;
                
        // Check all surrounding tiles to find path
        if ((dir == Left && x == 0)
             || (dir == Right && x == opt.mapWidth - 1)) {
            dir = Down;
        } else if (x > 0 && dir != Right
                  && ((*grid)[x-1][y] == Chunk_Main
                  ||  (*grid)[x-1][y] == Chunk_MainDrop)) {
            dir = Left;
        } else if (x < opt.mapWidth - 1 && dir != Left
                                   && ((*grid)[x+1][y] == Chunk_Main
                                   ||  (*grid)[x+1][y] == Chunk_MainDrop)) {
            dir = Right;
        } else if (y < opt.mapHeight - 1 && ((*grid)[x][y+1] == Chunk_MainLand
                                           ||  (*grid)[x][y+1] == Chunk_MainFall)) {
            dir = Down;
        } else if (y == opt.mapHeight - 1) {
            // End of path
            break;
        }
        
        // If chunk is an intersection, try to generate a degen path
        switch (chkType) {
            case Chunk_MainDrop:
                if (r->rand() < 0.4)
                    generateDegen(opt, grid, x, y, prevDir, r);
                if (r->rand() < 0.1)
                    generateDegen(opt, grid, x, y, Up, r);
                break;
            case Chunk_MainFall: {
                if (r->rand() < 0.5)
                    generateDegen(opt, grid, x, y, Left, r);
                if (r->rand() < 0.5)
                    generateDegen(opt, grid, x, y, Right, r);
                break;
            }
            case Chunk_MainLand:
                if (r->rand() < 0.4)
                    generateDegen(opt, grid, x, y, dir == Left ? Right : Left, r);
                break;
            case Chunk_Main:
                if (r->rand() < 0.1)
                    generateDegen(opt, grid, x, y, Up, r);
                if (r->rand() < 0.1)
                    generateDegen(opt, grid, x, y, Down, r);
            default:
                // Do nothing
                break;
        }
        
        switch (dir) {
            case Left:
                x--;
                break;
            case Right:
                x++;
                break;
            case Down:
                y++;
                break;
            case Up:
                y--;
                break;
        }
    }
}

void generateLevelDataFromGrid(const GeneratorOptions& opt,
                               vector<vector<ChunkType>> *grid,
                               LevelData *level,
                               Random *r) {
    level->init(opt);
    for (int x = 0; x < opt.mapWidth; x++) {
        for (int y = 0; y < opt.mapHeight; y++) {
            ChunkType type = (*grid)[x][y];
            auto templates = opt.templates.find(type)->second;
            int rand = r->rand(0, templates.size());
            level->chunks[x][y].generateTiles(templates[rand].chunk);
            level->chunks[x][y].generateMesh();
        }
    }
}

LevelData WWGenerator::generateLevel(const GeneratorOptions& opt) {
    // Seed random number generator
    srand(time(NULL));
    Random r(rand());
    
    LevelData level;
    vector<vector<ChunkType>> grid = generateGrid(opt, &level, &r);
    //generateDegenPaths(opt, &grid, &level, &r);
    
    generateLevelDataFromGrid(opt, &grid, &level, &r);
    return level;
}

