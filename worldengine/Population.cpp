//
// Created by adam on 6/6/2025.
//

#include "Population.h"

void Population::_sum_deltas(const Vector2 start, std::vector<Vector2> &temp) const {
    temp = this->rays_deltas;

    for (int i = 0; i < this->_rays_amount; i++) {
        temp[i].x += start.x;
        temp[i].y += start.y;
    }
}

float Population::_distance_compare(const Vector2 start, const Vector2 end) {
    return std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2);
}

Population::Population() {
    rays_deltas.resize(_rays_amount);
    const float k = 2.0f * M_PI / _rays_amount;
    for (int i = 0; i < _rays_amount; i++) {
        rays_deltas[i].x = std::cos(i * k) * _rays_radius;
        rays_deltas[i].y = std::sin(i * k) * _rays_radius;
    }
}

Population &Population::lines(const Lines &lines) {
    _lines = lines;
    return *this;
}

Population &Population::ants(const int ants_amount) {
    _ants_amount = ants_amount;
    return *this;
}

Population &Population::network(const std::vector<Layer> &layers, const std::string &filename) {
    _layers = layers;

    this->_ants.resize(_ants_amount, Ant(layers));

    this->_ants[0].network = Network(layers, filename);
    for (int i = 1; i < _ants_amount; i++) {
        this->_ants[i].network = Network(this->_ants[0].network);
    }

    return *this;
}

Population &Population::positions(const Vector2 init_position, const Vector2 target_position) {
    _init_position = init_position;
    _target_position = target_position;

    for (auto &ant: this->_ants) {
        ant.position = init_position;
    }

    return *this;
}

Population &Population::movement(const int move_method, const float max_speed) {
    _move_method = move_method;
    _max_speed = max_speed;
    return *this;
}

Population &Population::rays(const int rays_amount, const int rays_radius) {
    _rays_amount = rays_amount;
    _rays_radius = rays_radius;

    rays_deltas.resize(_rays_amount);
    const float k = 2.0f * M_PI / _rays_amount;
    for (int i = 0; i < _rays_amount; i++) {
        rays_deltas[i].x = std::cos(i * k) * _rays_radius;
        rays_deltas[i].y = std::sin(i * k) * _rays_radius;
    }

    return *this;
}

/* --- Main Logics Part --- */
void Population::calculateFrame() {
#pragma omp parallel for
    for (auto &ant: this->_ants) {
        // TODO
        std::vector<float> output = ant.network.feed_forward(std::vector<float>{0.1, 0.1, 0.1, 0.1});

        ant.position.x += output[0];
        ant.position.y += output[1];
    }
}

void Population::draw() {
    for (auto &ant: this->_ants) {
        ant.draw();
    }
}
