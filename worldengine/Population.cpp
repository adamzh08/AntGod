//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <iostream>

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

Population &Population::setNetwork(std::vector<Layer> &layers, const std::string &filename) {
    _layers = layers;

    this->_ants.resize(_ants_amount, Ant(this, RADIAL_MOVE, layers));

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

            ant.act();
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
                ant.rotation * RAD2DEG, // Rotation angle in degrees
                WHITE // Tint color
            );
        }
    }
    DrawCircleV(_init_position, 10, GREEN);
    DrawCircleV(_target_position, 10, RED);
}
