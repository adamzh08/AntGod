//
// Created by adam on 6/6/2025.
//

#ifndef ANT_H
#define ANT_H

#include "raylib.h"
#include "../neuralengine/Network.h"

class Population;

class Ant {
public:
    Population* population;
    Network network;
    Vector2 position{};
    int movementMode;
    float rotation = 0.0;
    bool alive = true;

    explicit Ant(Population* population, int movementMode, std::vector<Layer> &layers);

    void act();
    void draw();
};

#endif //ANT_H
