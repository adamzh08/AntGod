//
// Created by adam on 6/6/2025.
//

#include "Population.h"

Population::Population(const std::vector<Layer> &layers, const std::string &filename, const Lines &lines, const int populationSize, const int rays_amount, const int rays_length, const int move_method)
    : lines(lines), move_method(move_method), rays_amount(rays_amount), rays_length(rays_length) {

    this->ants.resize(populationSize, Ant(layers));

    this->ants[0].network = Network(layers, filename);

    for (int i = 1; i < populationSize; i++) {
        this->ants[i].network = Network(this->ants[0].network);
    }

    rays_deltas.resize(rays_amount);
    const float k = 2.0f * M_PI / rays_amount;
    for (int i = 0; i < rays_amount; i++) {
        rays_deltas[i].x = std::cos(i * k) * rays_length;
        rays_deltas[i].y = std::sin(i * k) * rays_length;
    }
}

void Population::sum_deltas(const Vector2 start, std::vector<Vector2> &temp) const {
    temp = this->rays_deltas;

    for (int i = 0; i < this->rays_amount; i++) {
        temp[i].x += start.x;
        temp[i].y += start.y;
    }
}

float Population::distance(const Vector2 start, const Vector2 end) {
    return std::sqrt(std::pow(start.x - end.x, 2) + std::pow(start.y - end.y, 2));
}

void Population::act() {
    for (auto& ant : this->ants) {

    }
}

/*

std::vector<Vector2D> rays_deltas(rays_amount);

int k = 2 * M_PI / rays_amount;
for (int i = 0; i < rays_amount; i++) {
    rays_deltas[i].x = cos(i * k) * rays_length;
    rays_deltas[i].y = sin(i * k) * rays_length;
}
*/