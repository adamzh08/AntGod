//
// Created by adam on 6/6/2025.
//

#include "Population.h"

Population::Population() {
    _rays_deltas.resize(_rays_amount);
    const float k = 2.0f * M_PI / _rays_amount;
    for (int i = 0; i < _rays_amount; i++) {
        _rays_deltas[i].x = std::cos(i * k) * _rays_radius;
        _rays_deltas[i].y = std::sin(i * k) * _rays_radius;
    }
}

Population &Population::setLines(const Lines &lines) {
    _lines = lines;
    return *this;
}

Population &Population::setAntTexture(const Texture2D &texture) {
    _antTexture = texture;
    return *this;
}

Population &Population::setAnts(const int ants_amount) {
    _ants_amount = ants_amount;
    return *this;
}

Population &Population::setNetwork(const std::vector<Layer> &layers, const std::string &filename) {
    _layers = layers;

    this->_ants.resize(_ants_amount, Ant(layers));

    this->_ants[0].network = Network(layers, filename);
    for (int i = 1; i < _ants_amount; i++) {
        this->_ants[i].network = Network(this->_ants[0].network);
    }

    return *this;
}

Population &Population::setPositions(const Vector2 init_position, const Vector2 target_position) {
    _init_position = init_position;
    _target_position = target_position;

    for (auto &ant: this->_ants) {
        ant.position = init_position;
    }

    return *this;
}

Population &Population::setMovement(const int move_method, const float max_speed) {
    _move_method = move_method;
    _max_speed = max_speed;
    return *this;
}

Population &Population::setRays(const int rays_amount, const int rays_radius) {
    _rays_amount = rays_amount;
    _rays_radius = rays_radius;

    _rays_deltas.resize(_rays_amount);
    const float k = 2.0f * M_PI / _rays_amount;
    for (int i = 0; i < _rays_amount; i++) {
        _rays_deltas[i].x = std::cos(i * k) * _rays_radius;
        _rays_deltas[i].y = std::sin(i * k) * _rays_radius;
    }

    return *this;
}

/* --- Main Logics Part --- */
void Population::act() {
#pragma omp parallel for
    for (auto &ant: this->_ants) {
        if (ant.alive) {
            // TODO

            std::vector<float> input(_rays_amount);

            for (int i = 0; i < _rays_amount; i++) {
                input[i] = _lines.get_intersection_delta(ant.position, _rays_deltas[i]);
                if (input[i] != 1000000) {
                    input[i] = (_rays_radius - input[i]) / _rays_radius;
                }
            }

            std::vector<float> output = ant.network.feed_forward(input);

            Vector2 temp_position{};

            if (_move_method == CARTESIAN_MOVE) {
                temp_position.x = ant.position.x + output[0] * _max_speed;
                temp_position.y = ant.position.y + output[1] * _max_speed;
                ant.direction = std::atan(output[1] / output[0]);
            } else if (_move_method == RADIAL_MOVE) {
                ant.direction += output[1] * PI;
                temp_position.x = ant.position.x + cos(ant.direction) * (output[0] + 1) / 2;
                temp_position.y = ant.position.y + sin(ant.direction) * (output[0] + 1) / 2;
            }

            if (temp_position.x > 1080) {
                ant.alive = false;
                continue;
            }
            if (temp_position.x < 0) {
                ant.alive = false;
                continue;
            }

            if (temp_position.y > 720) {
                ant.alive = false;
                continue;
            }
            if (temp_position.y < 0) {
                ant.alive = false;
                continue;
            }

            if (_lines.get_intersection(ant.position, temp_position) != 1000000) {
                ant.alive = false;
                continue;
            }

            ant.position = temp_position;
        }
    }
}

void Population::draw() const {

    if (_antTexture.id == 0) {
        std::cerr << "Texture not loaded!\n";
    }
    for (auto &ant: this->_ants) {
        if (ant.alive) {
            DrawTexturePro(
                _antTexture,
                (Rectangle){
                    0,
                    0,
                    static_cast<float>(_antTexture.width),
                    static_cast<float>(_antTexture.height)
                }, // Source rectangle
                (Rectangle){
                    ant.position.x,
                    ant.position.y,
                    static_cast<float>(_antTexture.width),
                    static_cast<float>(_antTexture.height)
                }, // Destination rectangle
                _origin_point, // Origin point for rotation
                ant.direction * 57.2957795, // Rotation angle in degrees
                WHITE // Tint color
            );
        }
    }
    DrawCircleV(_init_position, 10, GREEN);
    DrawCircleV(_target_position, 10, RED);
}
