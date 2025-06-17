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
    Population &_population;
    Network _network;

    Vector2 _position;
    float _rotation{};
    bool _alive = true;

    Ant(Population &population, const std::vector<Layer> &layers);
    Ant(const Ant& parent1, const Ant& parent2);

    Ant(const Ant& other);

    void act();

    void draw() const;

    [[nodiscard]] float calculateReward() const;
};

#endif //ANT_H
