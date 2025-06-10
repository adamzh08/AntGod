//
// Created by yanam on 10.06.2025.
//

#include "PopulationBuilder.h"

PopulationBuilder::PopulationBuilder(World &world): _world(world) {
}
#define REQUIRE_NOT_EMPTY(field, name) if (!field.has_value()) throw std::runtime_error(name " is empty, you have to set it to build a Population");

/**
 * @throws std::runtime_error if not all fields are set
 * @return a Population with the specified settings
 */
Population PopulationBuilder::build() {
    REQUIRE_NOT_EMPTY(_entityTexture, "texture");
    REQUIRE_NOT_EMPTY(_entityCount, "count");
    REQUIRE_NOT_EMPTY(_layers, "layers");
    REQUIRE_NOT_EMPTY(_filename, "file name");
    REQUIRE_NOT_EMPTY(_init_position, "init position");
    REQUIRE_NOT_EMPTY(_target_position, "target position");
    REQUIRE_NOT_EMPTY(_move_method, "movement method");
    REQUIRE_NOT_EMPTY(_max_speed, "speed");
    REQUIRE_NOT_EMPTY(_rays_amount, "ray amount");
    REQUIRE_NOT_EMPTY(_rays_radius, "ray radius");
    return Population(
        _world,
        _entityTexture.value(),
        _entityCount.value(),
        _layers.value(), _filename.value(),
        _init_position.value(), _target_position.value(),
        _move_method.value(), _max_speed.value(),
        _rays_amount.value(), _rays_radius.value()
    );
}


PopulationBuilder &PopulationBuilder::setEntityTexture(const Texture2D &texture) {
    _entityTexture = texture;
    return *this;
}

PopulationBuilder &PopulationBuilder::setCount(const int entity_count) {
    _entityCount = entity_count;
    return *this;
}

PopulationBuilder &PopulationBuilder::setNetwork(std::vector<Layer> &layers, const std::string &filename) {
    _layers = layers;
    _filename = filename;
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
