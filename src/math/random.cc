#include "Random.h"

#include <cstdlib>

const unsigned long a = 16807;       // 7^5
const unsigned long m = 2147483647;  // 2^32 - 1 (and thus prime)

// Schrage's algorithm constants

const uint64_t q = 127773;
const uint64_t r = 2836;

int64_t random_long(int64_t seed) {
    srand(seed);
    return rand();
    /*uint64_t k = 0;
    k = seed / q;
    seed = a * (seed - k * q) - r * k;
    
    if (seed < 0) {
      seed += m;
    }
    return seed;*/
}


Random::Random(int64_t seed) : pv(seed < 0 ? -seed : seed) {
    mult = 1.0 / (1.0 + (m-1));
}

void Random::seed(int64_t seed) {
    pv = seed;
}

int64_t Random::rand(int64_t min, int64_t max) {
    pv = random_long(pv);
    int64_t r = pv / (m / (max - min));
    return r + min;
}

float Random::rand() {
    pv = random_long(pv);
    return pv * mult;
}
