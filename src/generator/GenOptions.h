#ifndef GenOptions_hpp
#define GenOptions_hpp

#include "Generator.h"

typedef struct GenOptions {
    static GeneratorOptions TestGen;
    static GeneratorOptions TestCaveGen;
    
    static void init();
} GenOptions;

#endif  // GenOptions_hpp
