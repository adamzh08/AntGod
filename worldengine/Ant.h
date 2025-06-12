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
    Population *population;
    Network network;

    Vector2 position{};
    float rotation = 0.0;
    bool alive = true;

    Ant(Population &population, std::vector<Layer> &layers);
    Ant(const Ant& other);

    void act();

    void draw() const;

    [[nodiscard]] float calculateReward() const;
};

#endif //ANT_H
