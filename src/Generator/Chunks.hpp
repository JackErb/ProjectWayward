//
//  Chunks.hpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/7/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Chunks_hpp
#define Chunks_hpp

#include "../MathHelper/MathHelper.h"

#include <string>
#include <vector>

using std::vector;

typedef struct Tile {
    int type;
} TileData;

typedef enum ChunkType {
    Main, MainDrop, MainFall, MainLand, Degen, Standard
} ChunkType;

struct ChunkData {
    void Generate(const std::string& rep);
    
    int width;
    int height;
    int x;
    int y;
    
    // Two dimensional array of the chunk. Contains static & dynamic tiles during generation,
    // and only static tiles during gameplay.
    vector<vector<TileData>> data;
    
    // Collidable mesh of the static terrain.
    vector<PolygonV> mesh;
};

#endif  // Chunks_hpp
