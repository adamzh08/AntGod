//
// Created by adam on 6/6/2025.
//

#ifndef ANT_H
#define ANT_H

#include "raylib.h"
#include "../neuralengine/Network.h"

class Ant {
public:
    Network network;
    Vector2 position{};
    float direction = 0.0;
    bool alive = true;

    explicit Ant(const std::vector<Layer> &layers);
};

#endif //ANT_H
