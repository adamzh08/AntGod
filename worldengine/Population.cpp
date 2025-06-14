//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <cfloat>
#include <iostream>
#include <random>
#include <ranges>
#include <set>

Population::Population(
    World &world,
    int ants_amount,
    float elite_percentage,
    const Texture2D &texture,
    std::vector<Layer> &layers, const std::string &filename,
    Vector2 init_position, Vector2 target_position,
    int move_method, float max_speed, float max_angle,
    int rays_amount, int rays_radius): _world(world),
                                       _ants_amount(ants_amount),
                                       _elite_percentage(elite_percentage),
                                       _entityTexture(texture),
                                       _layers(layers), _filename(filename),
                                       _init_position(init_position), _target_position(target_position),
                                       _move_method(move_method), _max_speed(max_speed), _max_angle(max_angle),
                                       _rays_amount(rays_amount), _rays_radius(rays_radius) {
    // ants
    _ants.reserve(_ants_amount);

    if (filename.empty()) {
        // completely random
        for (int i = 0; i < _ants_amount; i++) {
            _ants.emplace_back(*this, layers);
        }
    } else {
        // from file
        Ant defaultAnt(*this, layers);
        for (int i = 0; i < _ants_amount; i++) {
            _ants.emplace_back(defaultAnt);
        }

        for (Ant &ant: this->_ants) {
            ant.network.mutate_weights(0.05, 0.1);
        }
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

void Population::flood() {
    std::vector<Ant *> selectedAnts;

    for (Ant &ant: _ants) {
        if (ant.alive) {
            selectedAnts.push_back(&ant);
        }
    }

    std::sort(selectedAnts.begin(), selectedAnts.end(), [](const Ant* ant1, const Ant* ant2) {
        return ant1->calculateReward() > ant2->calculateReward();
    });

    std::vector<Ant> nextGen;
    nextGen.reserve(_ants_amount);

    const int topX = static_cast<int>(_elite_percentage * selectedAnts.size());

    for (int i = 0; i < topX; ++i) {
        nextGen.push_back(Ant(*selectedAnts[i]));
    }



    for (int i = topX; i < _ants_amount; i++) {
        const int parentIndex = tournamentSelectFromPool(selectedAnts,
                                                         static_cast<int>(_ants_amount * 0.3));

        Ant child(*selectedAnts[parentIndex]);
        child.network.mutate_weights(0.01, 0.1);

        nextGen.push_back(child);
    }
    _ants = std::move(nextGen);
}

int Population::tournamentSelectFromPool(const std::vector<Ant *> &pool, const int k) {
    std::vector<int> indices(pool.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0..n-1
    std::ranges::shuffle(indices, std::mt19937{std::random_device{}()});

    std::vector<int> selectedIndexes(indices.begin(), indices.begin() + k);


    int best = -1;
    float highestReward = -FLT_MAX;

    for (int i = 0; i < k; i++) {
        const Ant *random = pool[selectedIndexes[i]];

        float reward = random->calculateReward();
        if (reward > highestReward) {
            highestReward = reward;
            best = selectedIndexes[i];
        }
    }
    return best;
}

void Population::draw() const {
    DrawCircleV(_init_position, 10, GREEN);
    DrawCircleV(_target_position, 10, RED);

    const Ant *best = nullptr;
    float highestReward = -FLT_MAX;

    for (const Ant &ant: this->_ants) {
        if (ant.alive) {
            float reward = ant.calculateReward();
            if (reward > highestReward) {
                highestReward = reward;
                best = &ant;
            }
        }
    }

    // drawing a blue circle on the best ant
    DrawEllipse(best->position.x, best->position.y, 10, 10, Color(0, 0, 255, 150));

    for (const Ant &ant: _ants) {
        if (ant.alive) {
            ant.draw();
        }
    }
}
