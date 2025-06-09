//
// Created by yanam on 10.06.2025.
//

#include "PopulationBuilder.h"

PopulationBuilder::PopulationBuilder(World& world): _world(world) {
}

Population PopulationBuilder::build() {
    return Population(_world, _entityTexture, _entityCount, _layers, _filename, _init_position, _target_position,
                      _move_method, _max_speed, _rays_amount, _rays_radius);
}


PopulationBuilder &PopulationBuilder::setEntityTexture(const Texture2D &texture) {
    _entityTexture = texture;
    _entityTexture = texture;
    return *this;
}

PopulationBuilder &PopulationBuilder::setAnts(const int ants_amount) {
    _entityCount = ants_amount;
    return *this;
}

PopulationBuilder &PopulationBuilder::setNetwork(std::vector<Layer> &layers, const std::string &filename) {
    _layers = layers;
    return *this;
}

PopulationBuilder &PopulationBuilder::setPositions(const Vector2 init_position, const Vector2 target_position) {
    _init_position = init_position;
    _target_position = target_position;
    return *this;
}

PopulationBuilder &PopulationBuilder::setMovement(const int move_method, const float max_speed) {
    _move_method = move_method;
    _max_speed = max_speed;
    return *this;
}

PopulationBuilder &PopulationBuilder::setRays(const int rays_amount, const int rays_radius) {
    _rays_amount = rays_amount;
    _rays_radius = rays_radius;
    return *this;
}
