//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <random>
#include <ranges>
#include <set>

#include "World.h"

Population::Population(
    World &world,
    int ants_amount,
    float elite_percentage,
    const Texture2D &texture,
    std::vector<Layer> &layers, const std::string &filename, float mutation_probability,
    Vector2 init_position, Vector2 target_position,
    int move_method, float max_speed, float max_angle,
    int rays_amount, int rays_radius): _world(world),
                                       _ants_amount(ants_amount),
                                       _elite_percentage(elite_percentage),
                                       _entityTexture(texture),
                                       _layers(layers), _filename(filename),
                                       _init_position(init_position), _target_position(target_position), _mutation_probability(mutation_probability),
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
            ant._network.mutate_weights(0.05, 0.1);
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

void Population::act() {
#pragma omp parallel for
    for (auto &ant: this->_ants) {
        if (ant._alive) {
            ant.act();
        }
    }
}

void Population::flood() {
    std::vector<Ant *> selectedAnts;

    for (Ant &ant: _ants) {
        if (ant._alive) {
            selectedAnts.push_back(&ant);
        }
    }
    std::ranges::sort(selectedAnts, [](const Ant *ant1, const Ant *ant2) {
        return ant1->calculateReward() > ant2->calculateReward();
    });

    std::vector<Ant> nextGen;
    nextGen.reserve(_ants_amount);


    // top X percent pass to the next gen without modification
    const int topX = static_cast<int>(_elite_percentage * selectedAnts.size());
    for (int i = 0; i < topX; ++i) {
        nextGen.push_back(Ant(*selectedAnts[i]));
    }

    // the other part are combinations of 2 parent ants
    for (int i = topX; i < _ants_amount; i++) {
        const int parentIndex1 = tournamentSelectFromPool(
            selectedAnts,
            static_cast<int>(selectedAnts.size() * 0.3)
        );
        const int parentIndex2 = tournamentSelectFromPool(
            selectedAnts,
            static_cast<int>(selectedAnts.size() * 0.3)
        );

        Ant child(*selectedAnts[parentIndex1], *selectedAnts[parentIndex2]);
        if (static_cast<double>(rand()) / RAND_MAX < _mutation_probability) {
            child._network.mutate_weights(0.01, 0.3);
        }

        nextGen.push_back(child);
    }
    // reset position and life status of all children
    for (Ant &child: nextGen) {
        child._alive = true;
        child._position = _init_position;
    }
    _ants = std::move(nextGen);
}

int Population::tournamentSelectFromPool(const std::vector<Ant *> &pool, const int k) {
    std::vector<int> indices(pool.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0..n-1
    std::ranges::shuffle(indices, std::mt19937{std::random_device{}()});

    const std::vector<int> selectedIndexes(indices.begin(), indices.begin() + k);


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
        if (ant._alive) {
            float reward = ant.calculateReward();
            if (reward > highestReward) {
                highestReward = reward;
                best = &ant;
            }
        }
    }

    if (best != nullptr) {
        // drawing a blue circle on the best ant
        DrawEllipse(best->_position.x, best->_position.y, 10, 10, Color(0, 0, 255, 150));
    } else {
        std::cerr << "[Warning] No more ants alive in population " << this << std::endl;
    }

    for (const Ant &ant: _ants) {
        if (ant._alive) {
            ant.draw();
            if (_world._showRays) {
                _world._lines.drawRays(ant._position, _rays_amount, _rays_radius);
            }
        }
    }
}
