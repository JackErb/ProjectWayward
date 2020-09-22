#include "FlowerGenerator.h"
#include <vector>

using std::vector;

Flower generateFlower() {
    Flower flower;
    flower.width = 1500;
    flower.height = 1500;

    Stem main_stem;
    main_stem.color = {139, 69, 19};
    main_stem.points.push_back({250, 0});
    main_stem.points.push_back({250, 600});

    Petal main_petal;
    main_petal.polygon = {{-40, 0}, {-40, 200}, {40, 200}, {40, 0}};
    main_petal.color = {128, 0, 0};
    
    Bud main_bud;
    main_bud.color = {255, 255, 51};
    main_bud.position = {250, 600};
    main_bud.radius = 150;

    main_bud.num_petals = 5;

    flower.stem = main_stem;
    flower.bud = main_bud;
    flower.bud.petal = main_petal;

    flower.texture_handle = generateFlowerTexture(flower);

    return flower;
}
