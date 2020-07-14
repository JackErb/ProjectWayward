//
//  Generator.hpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/7/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef Generator_hpp
#define Generator_hpp

#include "Chunks.hpp"

#include <string>
#include <vector>
#include <unordered_map>

struct LevelData;

typedef struct GeneratorOptions {
    int mapWidth;
    int mapHeight;
    
    int chunkWidth;
    int chunkHeight;
    
    int tileWidth;
    int tileHeight;
    
    // Temporary vector
    std::unordered_map<ChunkType, std::vector<ChunkTemplate>> *chunkTemplates;
} GeneratorOptions;

LevelData GenerateLevel(const GeneratorOptions& opt);

#endif  // Generator_hpp
