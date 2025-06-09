#include "Ant.h"

#include <iostream>

#include "Population.h"
#include "TextureCollection.h"
#include "World.h"

Ant::Ant(Population &population, std::vector<Layer> &layers): population(&population), network(layers) {}

void Ant::act() {
    std::vector<float> input(population->_rays_amount);

    for (int i = 0; i < population->_rays_amount; i++) {
        input[i] = population->_world->_lines.get_intersection_delta(position, population->_rays_deltas[i]);
        if (input[i] != 1000000) {
            input[i] = (population->_rays_radius - input[i]) / population->_rays_radius;
        }
    }

    const auto output = network.feed_forward(input);

    switch (population->_move_method) {
        case CARTESIAN_MOVE: {
            position.x += output[0] * population->_max_speed;
            position.y += output[1] * population->_max_speed;
            rotation = std::atan2(output[1], output[0]);
            break;
        }
        case RADIAL_MOVE: {
            rotation += output[1] * PI / 10;
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

void Ant::draw() const {
    DrawTextureEx(population->_entityTexture, position, rotation * RAD2DEG, 1, WHITE);
}
