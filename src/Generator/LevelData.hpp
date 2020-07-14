//
//  LevelData.hpp
//  ProjectWayward
//
//  Created by Jack Erb on 7/7/20.
//  Copyright © 2020 Jack Erb. All rights reserved.
//

#ifndef LevelData_hpp
#define LevelData_hpp

#include "Chunks.hpp"

class Random;
struct GeneratorOptions;

typedef struct LevelData {
    vector<vector<ChunkData>> chunks;
    int start_x;
    int map_w;
    int map_h;
    
    void Init(const GeneratorOptions& opt);
    void PrintChunks();
} LevelData;

#endif  // LevelData_hpp
