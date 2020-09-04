#include "GenOptions.h"

GeneratorOptions GenOptions::TestGen;
GeneratorOptions GenOptions::TestCaveGen;

void GenOptions::init() {
    /* TestGen Init */
    TestGen.mapWidth = 4;
    TestGen.mapHeight = 4;
    
    TestGen.chunkWidth = 10;
    TestGen.chunkHeight = 10;
    
    TestGen.tileWidth = 100;
    TestGen.tileHeight = 100;
    
    
    TestGen.templates[Chunk_Main] = {
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "oooooooooo")
    };
    
    TestGen.templates[Chunk_MainDrop] = {
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          ")
    };
    
    TestGen.templates[Chunk_MainFall] = {
        ChunkTemplate(std::string("")
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          ")
    };
    
    TestGen.templates[Chunk_MainLand] = {
        ChunkTemplate(std::string("")
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "oooooooooo")
    };
    
    TestGen.templates[Chunk_Standard] = {
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo")
    };
    
    TestGen.templates[Chunk_Degen] = {
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "ooo    ooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo")
    };
    
    
    
    
    
    /* TestCaveGen Init */
    
    TestCaveGen.mapWidth = 8;
    TestCaveGen.mapHeight = 10;
    
    TestCaveGen.chunkWidth = 10;
    TestCaveGen.chunkHeight = 10;
    
    TestCaveGen.tileWidth = 200;
    TestCaveGen.tileHeight = 200;
    
    TestCaveGen.templates[Chunk_Main] = {
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "o        o"
                      + "          "
                      + "  ooooo   "
                      + "  oooooo  "
                      + "          "
                      + "          "
                      + "          "
                      + "o         "
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "o        o"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "  ooooooo "
                      + "oooooooooo"
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooooooo   "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "      oo  "
        + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "ooo      o"
                      + "oo    oooo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "  oo      "
                      + "oooooo    "
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "        oo"
        + "          "
        + "          "
        + "          "
        + "     oo   "
        + "   oooo   "
        + "o        o"
        + "oo       o"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo       "
        + "o       oo"
        + "          "
        + "   ooooo  "
        + "  oooooo  "
        + "   oooo   "
        + "          "
        + "o         "
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo    ooo"
        + "o         "
        + "          "
        + "          "
        + " oooooooo "
        + "oooooooooo"
        + "      oooo"
        + "      oooo"
        + "oooooooooo")
    };
    
    TestCaveGen.templates[Chunk_MainDrop] = {
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooo      "
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "o         "
                      + "ooo       "
                      + "ooo      o"
                      + "ooo    ooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "o         "
                      + "ooo       "
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "oo        "
                      + "ooo   oooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "ooo     oo"
        + "          "
        + "          "
        + "  ooo     "
        + "ooooo  oo "
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo  ooooo")
    };
    
    TestCaveGen.templates[Chunk_MainFall] = {
        ChunkTemplate(std::string("")
                      + "ooo    ooo"
                      + "oo      oo"
                      + "o       oo"
                      + "o         "
                      + "o  ooo    "
                      + "   oooo   "
                      + "          "
                      + "o       oo"
                      + "o      ooo"
                      + "o      ooo"),
        ChunkTemplate(std::string("")
                    + "oo       o"
                    + "o        o"
                    + "o        o"
                    + "o         "
                    + "o         "
                    + "o         "
                    + "oo      oo"
                    + "ooo    ooo"
                    + "ooo    ooo"
                    + "ooo    ooo"),
        ChunkTemplate(std::string("")
        + "ooo      o"
        + "ooo      o"
        + "ooo       "
        + "oo        "
        + "oooooo    "
        + "ooooo    o"
        + "oo      oo"
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo    ooo")
    };
        
    TestCaveGen.templates[Chunk_MainLand] = {
        ChunkTemplate(std::string("")
                      + "ooo    ooo"
                      + "oo     ooo"
                      + "o        o"
                      + "          "
                      + "          "
                      + "          "
                      + "   oooo   "
                      + "          "
                      + "   ooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "o      ooo"
                      + "o      ooo"
                      + "       ooo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "oooooooooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "ooo    ooo"
        + "ooo    ooo"
        + "          "
        + "          "
        + "  ooo     "
        + "   ooo    "
        + "oo ooooo  "
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "   ooooo  "
        + "oooooooooo"
        + "oooooooooo"),
    };
    
    TestCaveGen.templates[Chunk_Degen] = {
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "          "
        + "   ooooo  "
        + "oooooooooo"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo    ooo"
        + "o        o"
        + "          "
        + "         o"
        + "         o"
        + "        oo"
        + "oooo   ooo"
        + "ooo   oooo"
        + "ooo   oooo"),
        ChunkTemplate(std::string("")
                      + "oooo  oooo"
                      + "oooo  oooo"
                      + "ooo   oooo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "o      ooo"
                      + "oo     ooo"
                      + "oooooooooo"),
        ChunkTemplate(std::string("")
                      + "oooooooooo"
                      + "oooooooooo"
                      + "ooo     oo"
                      + "          "
                      + "          "
                      + "          "
                      + "          "
                      + "oo    oooo"
                      + "oo    oooo"
                      + "ooo   oooo"),
        ChunkTemplate(std::string("")
        + "ooo    ooo"
        + "oo      oo"
        + "          "
        + "          "
        + "          "
        + "     oo   "
        + "   oooo   "
        + "o        o"
        + "oo       o"
        + "oooooooooo"),
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "ooo   oooo"
        + "o       oo"
        + "          "
        + "   ooooo  "
        + "  oooooo  "
        + "   oooo   "
        + "          "
        + "o         "
        + "oooooooooo")
    };
    
    TestCaveGen.templates[Chunk_Standard] = {
        ChunkTemplate(std::string("")
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo"
        + "oooooooooo")
    };
}
