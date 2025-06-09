#include "Ant.h"

#include "Population.h"
#include "TextureCollection.h"

Ant::Ant(Population *population, int movementMode, std::vector<Layer> &layers): population(population),
    movementMode(movementMode), network(layers) {
}

void Ant::act() {
    std::vector<float> input(population->_rays_amount);

    for (int i = 0; i < population->_rays_amount; i++) {
        input[i] = population->_lines.get_intersection_delta(position, population->_rays_deltas[i]);
        if (input[i] != 1000000) {
            input[i] = (population->_rays_radius - input[i]) / population->_rays_radius;
        }
    }

    std::vector<float> output = network.feed_forward(input);

    Vector2 temp_position{};

    if (population->_move_method == CARTESIAN_MOVE) {
        temp_position.x = position.x + output[0] * population->_max_speed;
        temp_position.y = position.y + output[1] * population->_max_speed;
        rotation = std::atan(output[1] / output[0]);
    } else if (population->_move_method == RADIAL_MOVE) {
        rotation += output[1] * PI;
        temp_position.x = position.x + cos(rotation) * (output[0] + 1) / 2;
        temp_position.y = position.y + sin(rotation) * (output[0] + 1) / 2;
    }

    if (temp_position.x > 1080) {
        alive = false;
        return;
    }
    if (temp_position.x < 0) {
        alive = false;
        return;
    }

    if (temp_position.y > 720) {
        alive = false;
        return;
    }
    if (temp_position.y < 0) {
        alive = false;
        return;
    }

    if (population->_lines.get_intersection(position, temp_position) != 1000000) {
        alive = false;
        return;
    }

    position = temp_position;
}

void Ant::draw() {
    DrawTextureEx(TextureCollection::ant, position, rotation * RAD2DEG, 1, WHITE);
}
