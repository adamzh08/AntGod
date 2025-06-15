#include "Ant.h"

#include <assert.h>
#include <iostream>

#include "Population.h"
#include "TextureCollection.h"
#include "World.h"

Ant::Ant(Population &population, const std::vector<Layer> &layers): _population(population), _network(layers),
                                                                    _position(population._init_position) {
}

Ant::Ant(const Ant &other) : _population(other._population), _network(other._network),
                             _position(other._population._init_position) {
}

Ant::Ant(const Ant &parent1, const Ant &parent2) : _population(parent1._population), _network(parent1._network),
                                                   _position(_population._init_position) {
    assert(&parent1._population == &parent2._population);
    _network.replaceWeightsWithOther(parent2._network, 0.5);
}


void Ant::act() {
    // Todo make rays work
    /*
    std::vector<float> input(population->_rays_amount);

    for (int i = 0; i < population->_rays_amount; i++) {
        input[i] = population->_world->_lines.get_intersection_delta(position, population->_rays_deltas[i]);
        if (input[i] != 1000000) {
            input[i] = (population->_rays_radius - input[i]) / population->_rays_radius;
        }
    }
    */

    std::vector<float> input{
        _position.x / 1000.f,
        _position.y / 1000.f,
    };

    const std::vector<float> output = _network.feed_forward(input);

    switch (_population._move_method) {
        case CARTESIAN_MOVE: {
            _position.x += output[0] * _population._max_speed;
            _position.y += output[1] * _population._max_speed;
            _rotation = std::atan2(output[1], output[0]);
            break;
        }
        case RADIAL_MOVE: {
            const float speedOutput = output[0] * _population._max_speed;
            const float rotationOutput = output[1] * _population._max_angle;
            _rotation += rotationOutput;
            _position.x += cos(_rotation) * speedOutput;
            _position.y += sin(_rotation) * speedOutput;
            break;
        }
        default:
            std::cerr << "Invalid movement mode" << std::endl;
    }

    if (_position.x < 0 || _position.x > GetScreenWidth() - _population._world._space_right
        || _position.y < 0 || _position.y > GetScreenHeight() - _population._world._space_bottom) {
        _alive = false;
    }
}

float Ant::calculateReward() const {
    const float dx = _population._target_position.x - _position.x;
    const float dy = _population._target_position.y - _position.y;
    return -(dx * dx + dy * dy);
}

void Ant::draw() const {
    DrawTextureEx(_population._entityTexture, _position, _rotation * RAD2DEG, 1, WHITE);
}
