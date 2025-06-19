//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <algorithm>
#include <cfloat>
#include <iostream>
#include <random>
#include <ranges>

#include "TextureCollection.h"
#include "World.h"

Population::Population(
    World &world,
    int ants_amount,
    float elite_percentage,
    const Texture2D &texture, const Color color,
    std::vector<Layer> &layers, const std::string &filename, float mutation_probability, float mutation_strength,
    Vector2 init_position, Vector2 target_position,
    int move_method, float max_speed, float max_angle,
    int rays_amount, int rays_radius, float rays_fov): _world(world),
                                       _ants_amount(ants_amount),
                                       _elite_percentage(elite_percentage),
                                       _entityTexture(texture), _entityColor(color),
                                       _layers(layers), _filename(filename),
                                       _init_position(init_position), _target_position(target_position),
                                       _mutation_probability(mutation_probability), _mutation_strength(mutation_strength),
                                       _move_method(move_method), _max_speed(max_speed), _max_angle(max_angle),
                                       _rays_amount(rays_amount), _rays_radius(rays_radius), _rays_fov(rays_fov) {
    // ants
    _ants.reserve(_ants_amount);

    if (filename.empty()) {
        // completely random
        for (int i = 0; i < _ants_amount; i++) {
            _ants.push_back(std::make_shared<Ant>(*this, layers));
        }
    } else {
        // from file
        Ant defaultAnt(*this, layers);
        for (int i = 0; i < _ants_amount; i++) {
            _ants.push_back(std::make_shared<Ant>(defaultAnt));
        }

        for (auto &ant: this->_ants) {
            ant->_network.mutate_weights(0.05, 0.1);
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
        if (ant->_alive) {
            ant->act();
        }
    }
}

void Population::flood() {
    _best = nullptr;

    std::vector<std::shared_ptr<Ant> > selectedAnts;
    selectedAnts.reserve(_ants_amount);
    for (auto &ant: _ants) {
        if (ant->_alive) {
            selectedAnts.push_back(ant);
        }
    }
    if (selectedAnts.empty()) {
        std::cerr << "Population " << this << " went extinct" << std::endl;
        return;
    }
    std::ranges::sort(selectedAnts, [](const std::shared_ptr<Ant> &a, const std::shared_ptr<Ant> &b) {
        return a->calculateReward() > b->calculateReward();
    });

    std::vector<std::shared_ptr<Ant>> nextGen;
    nextGen.reserve(_ants_amount);


    // top X percent pass to the next gen without modification
    const int topX = static_cast<int>(_elite_percentage * selectedAnts.size());
    for (int i = 0; i < topX; ++i) {
        nextGen.push_back(std::make_shared<Ant>(*selectedAnts[i]));
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
            child._network.mutate_weights(0.01, _mutation_strength);
        }

        nextGen.push_back(std::make_shared<Ant>(std::move(child)));
    }
    _ants = std::move(nextGen);
}

int Population::tournamentSelectFromPool(const std::vector<std::shared_ptr<Ant>> &pool, const int k) {
    std::vector<int> indices(pool.size());
    std::iota(indices.begin(), indices.end(), 0); // Fill with 0..n-1
    std::ranges::shuffle(indices, std::mt19937{std::random_device{}()});

    const std::vector<int> selectedIndexes(indices.begin(), indices.begin() + k);


    int best = -1;
    float highestReward = -FLT_MAX;

    for (int i = 0; i < k; i++) {
        const std::shared_ptr<Ant> random = pool[selectedIndexes[i]];

        float reward = random->calculateReward();
        if (reward > highestReward) {
            highestReward = reward;
            best = selectedIndexes[i];
        }
    }
    return best;
}

void Population::draw() {
    for (const auto &ant: _ants) {
        if (ant->_alive) {
            ant->draw();
        }
    }

    setBest();


    if (_best != nullptr) {
        if (_world._showRays) {
            _world._lines.drawRays(_best->_position, _rays_amount, _rays_radius);
        }
        // drawing a blue circle on the best ant
        DrawEllipse(_best->_position.x, _best->_position.y, 10, 10, Color(0, 0, 255, 150));
    } else {
        std::cerr << "[Warning] No more ants alive in population " << this << std::endl;
    }

    // init position
    drawXAt(_init_position);

    // target position
    drawFlagAt(_target_position);
}

void Population::drawXAt(const Vector2 pos) const {
    const Rectangle dest(
        pos.x,
        pos.y,
        TextureCollection::whiteX.width * 0.5,
        TextureCollection::whiteX.height * 0.5
    );
    DrawTexturePro(
        TextureCollection::whiteX,
        Rectangle(
            0,
            0,
            TextureCollection::whiteX.width,
            TextureCollection::whiteX.height
        ),
        dest,
        Vector2(dest.width / 2, dest.height / 2),
        0,
        _entityColor
    );
}

void Population::drawFlagAt(const Vector2 pos) const {
    const Rectangle dest(
        pos.x,
        pos.y,
        TextureCollection::finishFlag.width * 0.15,
        TextureCollection::finishFlag.height * 0.15
    );
    DrawTexturePro(
        TextureCollection::finishFlag,
        Rectangle(
            0,
            0,
            TextureCollection::finishFlag.width,
            TextureCollection::finishFlag.height
        ),
        dest,
        Vector2(
            0, dest.height
        ),
        0,
        _entityColor
    );
}


void Population::setBest() {
    float highestReward = -FLT_MAX;

    for (auto &ant: this->_ants) {
        if (ant->_alive) {
            float reward = ant->calculateReward();
            if (reward > highestReward) {
                highestReward = reward;
                _best = ant;
            }
        }
    }
}


int Population::getAliveCount() const {
    int counter = 0;
    for (auto &ant: _ants) {
        if (ant->_alive) {
            counter++;
        }
    }
    return counter;
}

float Population::getAvgDist() const {
    float sum = 0;
    for (auto &ant: _ants) {
        if (ant->_alive) {
            const float dx = ant->_position.x - _target_position.x;
            const float dy = ant->_position.y - _target_position.y;
            sum += std::sqrt(dx * dx + dy * dy);
        }
    }
    return sum / getAliveCount();
}

float Population::getBestDist() const {
    if (_best == nullptr) return FLT_MAX;
    const float dx = _best->_position.x - _target_position.x;
    const float dy = _best->_position.y - _target_position.y;
    return std::sqrt(dx * dx + dy * dy);
}
