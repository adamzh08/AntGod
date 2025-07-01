//
// Created by yanam on 10.06.2025.
//

#include "PopulationBuilder.h"

PopulationBuilder::PopulationBuilder(World &world): _world(world) {
}

std::unique_ptr<Population> PopulationBuilder::build() {
    return std::make_unique<Population>(
        Population(
            _world,
            _entityCount.value(),
            _elite_percentage.value(),
            _entityTexture.value(), _entityColor.value(),
            _mutation_newConnection_probability.value(), _mutation_newNeuron_probability.value(),
            _mutation_connection_probability.value(), _mutation_connection_strength.value(),
            _init_position.value(), _target_position.value(),
            _move_method.value(), _max_speed.value(), _max_angle.value(),
            _rays_amount.value(), _rays_radius.value(), _rays_fov.value()
        )
    );
}


PopulationBuilder &PopulationBuilder::setEntityTexture(const Texture2D &texture, const Color color) {
    _entityTexture = texture;
    _entityColor = color;
    return *this;
}

PopulationBuilder &PopulationBuilder::setCount(const int entity_count) {
    _entityCount = entity_count;
    return *this;
}

PopulationBuilder &PopulationBuilder::setElitePercentage(float elite_percentage) {
    _elite_percentage = elite_percentage;
    return *this;
}

PopulationBuilder &PopulationBuilder::setPositions(const Vector2 init_position, const Vector2 target_position) {
    _init_position = init_position;
    _target_position = target_position;
    return *this;
}

PopulationBuilder &PopulationBuilder::setMovement(const int move_method, const float max_speed, float max_angle) {
    _move_method = move_method;
    _max_speed = max_speed;
    _max_angle = max_angle;
    return *this;
}

PopulationBuilder &PopulationBuilder::setRays(const int rays_amount, const int rays_radius, const float rays_fov) {
    _rays_amount = rays_amount;
    _rays_radius = rays_radius;
    _rays_fov = rays_fov;
    return *this;
}

PopulationBuilder &PopulationBuilder::setMutation(float mutation_newConnection_probability,
                                                  float mutation_newNeuron_probability,
                                                  float mutation_connection_probability,
                                                  float mutation_connection_strength) {
    _mutation_newConnection_probability = mutation_newConnection_probability;
    _mutation_newNeuron_probability = mutation_newNeuron_probability;
    _mutation_connection_probability = mutation_connection_probability;
    _mutation_connection_strength = mutation_connection_strength;
    return *this;
}
