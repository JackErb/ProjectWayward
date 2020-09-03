set(SOURCES Generator.h Generator.cc Chunks.h Chunks.cc LevelData.h
            LevelData.cc GenOptions.h GenOptions.cc)

add_library(ww_generator STATIC ${SOURCES})
