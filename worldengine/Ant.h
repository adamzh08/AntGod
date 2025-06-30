//
// Created by adam on 6/6/2025.
//

#ifndef ANT_H
#define ANT_H

#include "raylib.h"
#include "../neuralengine/NEAT_Network.h"

class Population;

class Ant {
public:
    Population &_population;
    NEAT_Network _network;

    Vector2 _position;
    float _rotation = PI;
    bool _alive = true;

    int _framesAlive{};

    Ant(Population &population, int inputCount, int outputCount);
    // Ant(const Ant& parent1, const Ant& parent2); // Todo

    explicit Ant(const Ant& other);

    void act();

    void draw() const;
    [[nodiscard]] float calculateReward() const;
};

#endif //ANT_H
