#ifndef LevelData_hpp
#define LevelData_hpp

#include "Chunks.h"

class Random;
struct GeneratorOptions;

typedef struct LevelData {
    vector<vector<ChunkData>> chunks;
    int start_x;
    int map_w;
    int map_h;
    
    void init(const GeneratorOptions& opt);
    void printChunks();
} LevelData;

#endif  // LevelData_hpp
