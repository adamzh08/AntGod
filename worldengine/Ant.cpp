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
        _population._rays_amount,
        _population._rays_radius
    );
    const std::vector<float> output = _network.feed_forward(input);

    const Vector2 temp = _position;
    const float rot_temp = _rotation;

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

    if (!_population._world._lines.validMove(temp, _position)) {
        _position = temp;
        _rotation = rot_temp;
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
