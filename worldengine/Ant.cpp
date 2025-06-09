#include "Ant.h"

#include "TextureCollection.h"

Ant::Ant(const std::vector<Layer> &layers): network(layers) {}


void Ant::draw() {
    DrawTextureEx(TextureCollection::ant, position, 0, 1, WHITE);
}

