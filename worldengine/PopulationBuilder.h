//
// Created by yanam on 10.06.2025.
//

#ifndef POPULATIONBUILDER_H
#define POPULATIONBUILDER_H
#include <optional>

#include "Population.h"
#include "raylib.h"
#include "World.h"


class PopulationBuilder {
public:
    World &_world;
    std::optional<int> _entityCount;
    std::optional<float> _elite_percentage;

    // Ant settings
    std::optional<Texture2D> _entityTexture;
    std::optional<Color> _entityColor;

    // Neural network
    std::optional<float> _mutation_newConnection_probability;
    std::optional<float> _mutation_newNeuron_probability;
    std::optional<float> _mutation_connection_probability;
    std::optional<float> _mutation_connection_strength;

    // Positions
    std::optional<Vector2> _init_position;
    std::optional<Vector2> _target_position;

    // Movement
    std::optional<int> _move_method;
    std::optional<float> _max_speed;
    std::optional<float> _max_angle;

    // Rays
    std::optional<int> _rays_amount;
    std::optional<int> _rays_radius;
    std::optional<int> _rays_fov;


    explicit PopulationBuilder(World& world);

    PopulationBuilder& setEntityTexture(const Texture2D &texture, const Color color);
    PopulationBuilder& setCount(int ants_amount);
    PopulationBuilder& setPositions(Vector2 init_position, Vector2 target_position);
    PopulationBuilder& setMovement(int move_method, float max_speed, float max_angle);
    PopulationBuilder& setRays(int rays_amount, int rays_radius, float rays_fov);
    PopulationBuilder& setElitePercentage(float elite_percentage);
    PopulationBuilder& setMutation(float mutation_newConnection_probability, float mutation_newNeuron_probability, float mutation_connection_probability, float mutation_connection_strength);


    std::unique_ptr<Population> build();
};



#endif //POPULATIONBUILDER_H
