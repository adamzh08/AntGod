#include "Ant.h"

#include "Textures.h"

Ant::Ant(const std::vector<Layer> &layers): network(layers) {}


void Ant::draw() {
    DrawTextureEx(Textures::ant, position, rotation, 1, WHITE);
}

