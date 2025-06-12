//
// Created by adam on 6/6/2025.
//

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>

#include "Ant.h"
#include "Line.h"
#include "Lines.h"
#include "raylib.h"

#define CARTESIAN_MOVE 0
#define RADIAL_MOVE 1

class World;

class Population {
public:
    World *_world;

    // -------- Population ---------
    std::vector<Ant> _ants;
    int _ants_amount{};

    // Ant settings
    Texture2D _entityTexture;
    Vector2 _origin_point;

    // Neural network
    std::vector<Layer> _layers;
    std::string _filename;

    // Positions
    Vector2 _init_position;
    Vector2 _target_position;

    // Movement
    int _move_method;
    float _max_speed;
    float _max_angle;

    // Rays
    std::vector<Vector2> _rays_deltas;
    int _rays_amount;
    int _rays_radius;


    Population(
        World &world,
        const Texture2D &texture,
        int ants_amount,
        std::vector<Layer> &layers, const std::string &filename,
        Vector2 init_position, Vector2 target_position,
        int move_method, float max_speed, float max_angle,
        int rays_amount, int rays_radius
    );

    void act();

    void flood();

    static Ant tournamentSelectFromPool(const std::vector<Ant> &pool, const int k);

    void draw() const;
};

#endif //POPULATION_H
