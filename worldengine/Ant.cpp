#include "Ant.h"

#include <iostream>

#include "Population.h"
#include "TextureCollection.h"
#include "World.h"

Ant::Ant(Population &population, std::vector<Layer> &layers): population(&population), network(layers) {}

Ant::Ant(const Ant &other) : population(other.population), network(other.network), position(other.population->_init_position) {}


void Ant::act() {
    // std::vector<float> input(population->_rays_amount);

    std::vector<float> input {
        position.x / 1000,
        position.y / 1000,
    };

    /*
    for (int i = 0; i < population->_rays_amount; i++) {
        input[i] = population->_world->_lines.get_intersection_delta(position, population->_rays_deltas[i]);
        if (input[i] != 1000000) {
            input[i] = (population->_rays_radius - input[i]) / population->_rays_radius;
        }
    }
    */

    const auto output = network.feed_forward(input);

    switch (population->_move_method) {
        case CARTESIAN_MOVE: {
            position.x += output[0] * population->_max_speed;
            position.y += output[1] * population->_max_speed;
            rotation = std::atan2(output[1], output[0]);
            break;
        }
        case RADIAL_MOVE: {
            rotation += output[1] * population->_max_angle;
            const float speed = (output[0] + 1) / 2 * population->_max_speed;
            position.x += cos(rotation) * speed;
            position.y += sin(rotation) * speed;
            break;
        }
        default:
            std::cerr << "Invalid movement mode" << std::endl;
    }
    if (position.x > 1080 || position.x < 0 || position.y > 720 || position.y < 0) {
        alive = false;
    }
}

float Ant::calculateReward() const {
    const float dx = population->_target_position.x - position.x;
    const float dy = population->_target_position.y - position.y;
    return -(dx * dx + dy * dy);
}

void Ant::draw() const {
    DrawTextureEx(population->_entityTexture, position, rotation * RAD2DEG, 1, WHITE);
}
