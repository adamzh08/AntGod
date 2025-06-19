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
    // Todo @Adam integrate _rotation and _population._rays_fov
    const std::vector<float> input = _population._world._lines.getRays(
        _position,
        _population._rays_radius,
        _population._rays_amount,
        _rotation + PI,
        1
    );
    const std::vector<float> output = _network.feed_forward(input);

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
}   else {
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
        Rectangle(0, 0, _population._entityTexture.width, _population._entityTexture.height), // Source rectangle
        Rectangle(_position.x, _position.y, _population._entityTexture.width, _population._entityTexture.height),
        // Destination rectangle
        Vector2(_population._entityTexture.width / 2.0f, _population._entityTexture.height / 2.0f),
        // Origin point for rotation
        _rotation * RAD2DEG, // Rotation angle in degrees
        _population._entityColor // Tint color
    );
}
