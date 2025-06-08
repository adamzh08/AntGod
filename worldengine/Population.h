//
// Created by adam on 6/6/2025.
//

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <iostream>

#include "Ant.h"
#include "Line.h"
#include "Lines.h"

#define CARTESIAN_MOVE 0
#define RADIAL_MOVE 1

class Population {
private:
    Lines _lines{};

    // Ant
    std::string _imagePath = "assets/ant.png";
    Texture2D _antTexture;
    Vector2 _origin_point{};

    // Population
    std::vector<Ant> _ants;
    int _ants_amount = 1000;

    // Neural network
    std::vector<Layer> _layers{};
    std::string _filename = "weights.bin";

    // Positions
    Vector2 _init_position{10, 10};
    Vector2 _target_position{20, 10};

    // Movement
    int _move_method = CARTESIAN_MOVE;
    float _max_speed = 1.0f;

    // Rays
    std::vector<Vector2> _rays_deltas{};
    int _rays_amount = 30;
    int _rays_radius = 100;
public:
    Population();

    // Settings
    Population& lines(const Lines &lines);
    Population& image(const std::string &imagePath);
    Population& ants(const int ants_amount);
    Population& network(const std::vector<Layer> &layers, const std::string &filename);
    Population& positions(const Vector2 init_position, const Vector2 target_position);
    Population& movement(const int move_method, const float max_speed);
    Population& rays(const int rays_amount, const int rays_radius);

    // Load the Texture in GPU
    void load_image();

    // Calculate
    void act();

    // Draw
    void draw() const;
};

#endif //POPULATION_H
