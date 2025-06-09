//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <iostream>

Population::Population(
    World &world,
    const Texture2D &texture,
    int ants_amount,
    std::vector<Layer> &layers, const std::string &filename,
    Vector2 init_position, Vector2 target_position,
    int move_method, float max_speed,
    int rays_amount, int rays_radius): _world(&world),
                                       _entityTexture(texture),
                                       _ants_amount(ants_amount),
                                       _layers(layers), _filename(filename),
                                       _init_position(init_position), _target_position(target_position),
                                       _move_method(move_method), _max_speed(max_speed),
                                       _rays_amount(rays_amount), _rays_radius(rays_radius) {
    // ants
    this->_ants.resize(_ants_amount, Ant(*this, layers));

    this->_ants[0].network = Network(layers, filename);
    for (int i = 1; i < _ants_amount; i++) {
        this->_ants[i].network = Network(this->_ants[0].network);
    }
    for (auto &ant: this->_ants) {
        ant.position = init_position;
    }

    // rays
    _rays_deltas.resize(_rays_amount);
    const float k = 2.0f * M_PI / _rays_amount;
    for (int i = 0; i < _rays_amount; i++) {
        _rays_deltas[i].x = std::cos(i * k) * _rays_radius;
        _rays_deltas[i].y = std::sin(i * k) * _rays_radius;
    }
}

/* --- Main Logics Part --- */
void Population::act() {
#pragma omp parallel for
    for (auto &ant: this->_ants) {
        if (ant.alive) {
            ant.act();
        }
    }
}

void Population::draw() const {
    DrawCircleV(_init_position, 10, GREEN);
    DrawCircleV(_target_position, 10, RED);

    for (const Ant &ant: this->_ants) {
        if (ant.alive) {
            ant.draw();
        }
    }
}
