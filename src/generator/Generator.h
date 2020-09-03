#ifndef Generator_hpp
#define Generator_hpp

#include "Chunks.h"

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
    std::unordered_map<ChunkType, std::vector<ChunkTemplate>> templates;
} GeneratorOptions;

namespace WWGenerator {

LevelData generateLevel(const GeneratorOptions& opt);

}

#endif  // Generator_hpp
