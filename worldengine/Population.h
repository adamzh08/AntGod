//
// Created by adam on 6/6/2025.
//

#ifndef POPULATION_H
#define POPULATION_H

#include <memory>
#include <vector>

#include "Ant.h"
#include "raylib.h"

#define CARTESIAN_MOVE 0
#define RADIAL_MOVE 1

class World;

class Population {
private:
    void setBest();

public:
    World &_world;

    // -------- Population ---------
    std::vector<std::unique_ptr<Ant> > _ants;
    int _bestIdx = -1;
    int _ants_amount;
    float _elite_percentage;

    // Ant settings
    Texture2D _entityTexture;
    Color _entityColor;
    Vector2 _origin_point;

    // Neural network
    std::vector<Layer> _layers;
    std::string _filename;
    float _mutation_probability;
    float _mutation_strength;

    // Positions
    Vector2 _init_position;
    Vector2 _target_position;

    // Movement
    int _move_method;
    float _max_speed;
    float _max_angle;

    // Rays
    int _rays_amount;
    int _rays_radius;
    float _rays_fov;


    Population(
        World &world,
        int ants_amount,
        float elite_percentage,
        const Texture2D &texture, Color color,
        std::vector<Layer> &layers, const std::string &filename, float mutation_probability, float _mutation_strength,
        Vector2 init_position, Vector2 target_position,
        int move_method, float max_speed, float max_angle,
        int rays_amount, int rays_radius, float rays_fov
    );

    void initAnts();

    void act();

    void flood();

    [[nodiscard]] int getAliveCount() const;

    [[nodiscard]] float getAvgDist() const;

    [[nodiscard]] float getBestDist() const;

    static int tournamentSelectFromPool(const std::vector<Ant *> &pool, int k);

    void draw();

    static void drawXAt(Vector2, Color);

    static void drawFlagAt(Vector2, Color);
};

#endif //POPULATION_H
