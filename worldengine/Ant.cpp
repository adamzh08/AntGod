#include "Ant.h"

#include "raylib.h"

#include <assert.h>
#include <cmath>
#include <iostream>

#include "Population.h"
#include "World.h"
#include "../neuralengine/NEAT_Network.h"

Ant::Ant(Population &population, const int inputCount, const int outputCount): _population(population),
                                                                               _network(inputCount, outputCount),
                                                                               _position(population._init_position) {
}

Ant::Ant(const Ant &other) :
_population(other._population),
                             _network(other._network),
                             _position(other._population._init_position) {
}

/* Todo
Ant::Ant(const Ant &parent1, const Ant &parent2) : _population(parent1._population), _network(parent1._network),
                                                   _position(_population._init_position) {
    assert(&parent1._population == &parent2._population);
}
*/


void Ant::act() {
    _framesAlive++;

    const std::vector<float> input = _population._world._lines.getRays(
        _position,
        _population._rays_radius,
        _population._rays_amount,
        _rotation + PI,
        _population._rays_fov
    );

    const std::vector<float> output = _network.FeedForward(input);

    for (int i = 0; i < output.size(); i++) {
        if (std::abs(output[i]) > 1) {
            std::cout << output[i] << std::endl;
        }
    }

    Vector2 temp = _position;
    float rot_temp = _rotation;

    switch (_population._move_method) {
        case CARTESIAN_MOVE: {
            temp.x += output[0] * _population._max_speed;
            temp.y += output[1] * _population._max_speed;
            rot_temp = std::atan2(output[1], output[0]);
            break;
        }
        case RADIAL_MOVE: {
            const float speedOutput = output[0] * _population._max_speed;
            const float rotationOutput = output[1] * _population._max_angle;
            rot_temp += rotationOutput;

            if (rot_temp < 0) {
                rot_temp += 2 * PI;
            } else if (rot_temp >= 2 * PI) {
                rot_temp -= 2 * PI;
            }

            temp.x += cos(rot_temp) * speedOutput;
            temp.y += sin(rot_temp) * speedOutput;
            break;
        }
        default:
            std::cerr << "Invalid movement mode" << std::endl;
    }

    if (_population._world._lines.validMove(_position, temp)) {
        _position = temp;
        _rotation = rot_temp;
    } else {
        _alive = false;
    }
}

float Ant::calculateReward() const {
    const float dx = _population._target_position.x - _position.x;
    const float dy = _population._target_position.y - _position.y;
    return -(dx * dx + dy * dy);
}

void Ant::draw() const {
    DrawTexturePro(
        _population._entityTexture,
        Rectangle{
            0, 0, static_cast<float>(_population._entityTexture.width),
            static_cast<float>(_population._entityTexture.height)
        }, // Source
        Rectangle{
            _position.x, // Center position
            _position.y,
            static_cast<float>(_population._entityTexture.width),
            static_cast<float>(_population._entityTexture.height)
        },
        Vector2{
            _population._entityTexture.width / 2.0f,
            _population._entityTexture.height / 2.0f
        }, // Origin: center of the sprite
        _rotation * RAD2DEG,
        _population._entityColor
    );
}
