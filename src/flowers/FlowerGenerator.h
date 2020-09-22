#ifndef FlowerGenerator_h
#define FlowerGenerator_h

#include <vector>

typedef struct Point {
    int x;
    int y;
} Point;

typedef struct Color {
    int r;
    int g;
    int b;
} Color;

typedef struct Stem {
    Color color;
    std::vector<Point> points;
} Stem;

typedef struct Petal {
    std::vector<Point> polygon;
    Color color;
} Petal;

typedef struct Bud {
    Color color;
    Point position;
    int radius;

    int num_petals;
    Petal petal;
} Bud;

typedef struct Flower {
    int width;
    int height;

    Stem stem;
    Bud bud;

    unsigned int texture_handle;
} Flower;

Flower generateFlower();
unsigned int generateFlowerTexture(const Flower &flower);

#endif
