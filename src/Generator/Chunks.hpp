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

typedef enum TileType {
    Tile_Static, Tile_Air
} TileType;

typedef struct TileData {
    TileType type;
    bool meshed;
} TileData;

typedef enum ChunkType {
    Chunk_Main, Chunk_MainDrop, Chunk_MainFall,
    Chunk_MainLand, Chunk_Degen, Chunk_Standard
} ChunkType;

typedef struct ChunkTemplate {
    ChunkTemplate(const std::string& rep, float w = 1.f) : weight(w), chunk(rep) {}
    
    float weight;
    std::string chunk;
} ChunkTemplate;

struct ChunkData {
    void GenerateTiles(const std::string& rep);
    
    // Must be called every time tile data is updated
    void GenerateMesh();
    
    // Number of tiles in chunk
    int chunk_w;
    int chunk_h;
    
    // Tile size in pixels
    int tile_w;
    int tile_h;
    
    // Chunk position in pixels
    int chunk_x;
    int chunk_y;
    
    // Two dimensional array of the chunk. Contains static & dynamic tiles during generation,
    // and only static tiles during gameplay.
    vector<vector<TileData>> tiles;
    
    // Collidable mesh of the static terrain.
    vector<PolygonV> mesh;
};

#endif  // Chunks_hpp
