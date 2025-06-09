//
// Created by yanam on 10.06.2025.
//

#ifndef POPULATIONBUILDER_H
#define POPULATIONBUILDER_H
#include "Population.h"
#include "raylib.h"


class PopulationBuilder {
public:
    World &_world;
    int _entityCount{};

    // Ant settings
    Texture2D _entityTexture;
    Vector2 _origin_point{};

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
    int _rays_amount = 30;
    int _rays_radius = 100;

    PopulationBuilder(World& world);

    PopulationBuilder& setEntityTexture(const Texture2D &texture);
    PopulationBuilder& setAnts(int ants_amount);
    PopulationBuilder& setNetwork(std::vector<Layer> &layers, const std::string &filename);
    PopulationBuilder& setPositions(Vector2 init_position, Vector2 target_position);
    PopulationBuilder& setMovement(int move_method, float max_speed);
    PopulationBuilder& setRays(int rays_amount, int rays_radius);

    Population build();
};



#endif //POPULATIONBUILDER_H
