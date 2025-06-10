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
    std::optional<int> _entityCount;

    // Ant settings
    std::optional<Texture2D> _entityTexture;
    std::optional<Vector2> _origin_point;

    // Neural network
    std::optional<std::vector<Layer>> _layers;
    std::optional<std::string> _filename;

    // Positions
    std::optional<Vector2> _init_position;
    std::optional<Vector2> _target_position;

    // Movement
    std::optional<int> _move_method;
    std::optional<float> _max_speed;

    // Rays
    std::optional<int> _rays_amount;
    std::optional<int> _rays_radius;



    PopulationBuilder(World& world);

    PopulationBuilder& setEntityTexture(const Texture2D &texture);
    PopulationBuilder& setCount(int ants_amount);
    PopulationBuilder& setNetwork(std::vector<Layer> &layers, const std::string &filename);
    PopulationBuilder& setPositions(Vector2 init_position, Vector2 target_position);
    PopulationBuilder& setMovement(int move_method, float max_speed);
    PopulationBuilder& setRays(int rays_amount, int rays_radius);

    Population build();
};



#endif //POPULATIONBUILDER_H
