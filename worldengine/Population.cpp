//
// Created by adam on 6/6/2025.
//

#include "Population.h"

#include <algorithm>
#include <cassert>
#include <cfloat>
#include <iostream>
#include <numeric>
#include <random>
#include <ranges>

#include "UI/TextureCollection.h"
#include "World.h"

class Layer;

Population::Population(
    World &world,
    int ants_amount,
    float elite_percentage,
    Texture2D &texture, Color color,
    float mutation_newConnection_probability, float mutation_newNeuron_probability,
    float mutation_connection_probability, float mutation_connection_strength,
    Vector2 init_position, Vector2 target_position,
    int move_method, float max_speed, float max_angle,
    int rays_amount, int rays_radius, float rays_fov): _ants_amount(ants_amount),
                                                       _world(world),
                                                       _elite_percentage(elite_percentage),
                                                       _entityTexture(texture), _entityColor(color),
                                                       _mutation_newConnection_probability(
                                                           mutation_newConnection_probability
                                                       ),
                                                       _mutation_newNeuron_probability(
                                                           mutation_newNeuron_probability
                                                       ),
                                                       _mutation_connection_probability(
                                                           mutation_connection_probability
                                                       ),
                                                       _mutation_connection_strength(
                                                           mutation_connection_strength
                                                       ),
                                                       _init_position(init_position),
                                                       _target_position(target_position),
                                                       _move_method(move_method), _max_speed(max_speed),
                                                       _max_angle(max_angle),
                                                       _rays_amount(rays_amount), _rays_radius(rays_radius),
                                                       _rays_fov(rays_fov) {
    _next_ants_amount = _ants_amount;
}

void Population::initAnts() {
    _ants.clear();
    _ants.reserve(_ants_amount);

    for (int i = 0; i < _ants_amount; i++) {
        _ants.push_back(std::make_unique<Ant>(*this, _rays_amount, 2));
        _ants.back()->_network.TryAddRandomConnection();
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
    _bestIdx = -1;
    _ants_amount = _next_ants_amount;

    std::vector<Ant *> selectedAnts;
    selectedAnts.reserve(_ants_amount);
    for (auto &ant: _ants) {
        if (ant->_alive) {
            selectedAnts.push_back(ant.get()); // raw pointer for sorting & selection
        }
    }
    if (selectedAnts.empty()) {
        std::cerr << "Population " << this << " went extinct" << std::endl;
        //  return; // Todo
        /*
        std::ranges::sort(_ants.begin(), _ants.end(), [](const std::unique_ptr<Ant>& a, const std::unique_ptr<Ant>& b) {
            return a->_framesAlive > b->_framesAlive;
        });
        selectedAnts.push_back(_ants[0].get());
        selectedAnts.push_back(_ants[1].get());
        selectedAnts.push_back(_ants[2].get());
        selectedAnts.push_back(_ants[3].get());
        selectedAnts.push_back(_ants[4].get());
*/
        for (auto &ant: _ants) {
            selectedAnts.push_back(ant.get());
        }
    }
    std::ranges::sort(selectedAnts, [](const Ant *a, const Ant *b) {
        return a->calculateReward() > b->calculateReward();
    });

    std::vector<std::unique_ptr<Ant> > nextGen;
    nextGen.reserve(_ants_amount);

    // Top X percent pass to next gen unchanged (deep copy)
    const int topX = static_cast<int>(_elite_percentage * selectedAnts.size());
    for (int i = 0; i < topX; ++i) {
        nextGen.push_back(std::make_unique<Ant>(*selectedAnts[i]));
    }

    // Other are offspring
    for (int i = topX; i < _ants_amount; ++i) {
        const int parentIndex1 = tournamentSelectFromPool(selectedAnts, static_cast<int>(selectedAnts.size() * 0.3));
        // const int parentIndex2 = tournamentSelectFromPool(selectedAnts, static_cast<int>(selectedAnts.size() * 0.3));

        // Ant child(*selectedAnts[parentIndex1], *selectedAnts[parentIndex2]);
        Ant child(*selectedAnts[parentIndex1]);
        // mutations
        if (static_cast<double>(rand()) / RAND_MAX < _mutation_connection_probability) {
            child._network.TryMutateRandomConnection(_mutation_connection_strength);
        }
        if (static_cast<double>(rand()) / RAND_MAX < _mutation_newConnection_probability) {
            child._network.TryAddRandomConnection();
        }
        if (static_cast<double>(rand()) / RAND_MAX < _mutation_newNeuron_probability) {
            child._network.TryAddRandomNeuron();
        }
        nextGen.push_back(std::make_unique<Ant>(std::move(child)));
    }
    for (auto &ant: nextGen) {
        ant->_position = _init_position;
    }
    _ants = std::move(nextGen);
}


int Population::tournamentSelectFromPool(const std::vector<Ant *> &pool, const int k) {
    std::vector<int> indices(pool.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::ranges::shuffle(indices, std::mt19937{std::random_device{}()});

    const std::vector<int> selectedIndexes(indices.begin(), indices.begin() + k);

    int best = -1;
    float highestReward = -FLT_MAX;

    for (int i = 0; i < k; ++i) {
        Ant *candidate = pool[selectedIndexes[i]];
        float reward = candidate->calculateReward();
        if (reward > highestReward) {
            highestReward = reward;
            best = selectedIndexes[i];
        }
    }
    return best;
}

void Population::draw() {
    for (const auto &ant: _ants) {
        if (ant != nullptr && ant->_alive) {
            ant->draw();
        }
    }
    setBest();

    if (_bestIdx != -1) {
        std::unique_ptr<Ant> &bestAnt = _ants[_bestIdx];
        if (_world._showRays) {
            _world._lines.drawRays(bestAnt->_position,
                                   _rays_radius,
                                   bestAnt->_rotation,
                                   _rays_fov,
                                   _rays_amount
            );
        }
        // drawing a star on the best ant
        const float scale = 0.5;
        DrawTextureEx(
            TextureCollection::whiteStar,
            Vector2{
                bestAnt->_position.x - (TextureCollection::whiteStar.width * scale) / 2.0f,
                bestAnt->_position.y - (TextureCollection::whiteStar.height * scale) / 2.0f
            },
            0,
            scale,
            Fade(_entityColor, 0.5)
        );
        // DrawEllipse(bestAnt->_position.x, bestAnt->_position.y, 10, 10, Color(0, 0, 255, 150));
    } else {
        std::cerr << "[Warning] No more ants alive in population " << this << std::endl;
    }

    // init position
    drawXAt(_init_position, _entityColor);

    // target position
    drawFlagAt(_target_position, _entityColor);
}

void Population::drawXAt(const Vector2 pos, const Color tint) {
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
        tint
    );
}

void Population::drawFlagAt(const Vector2 pos, const Color tint) {
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
        tint
    );
}


void Population::setBest() {
    float highestReward = -FLT_MAX;

    for (int i = 0; i < _ants_amount; i++) {
        if (i < _ants.size() && _ants[i]->_alive) {
            const float reward = _ants[i]->calculateReward();
            if (reward > highestReward) {
                highestReward = reward;
                _bestIdx = i;
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
    if (_bestIdx == -1) return FLT_MAX;
    const std::unique_ptr<Ant> &bestAnt = _ants[_bestIdx];
    const float dx = bestAnt->_position.x - _target_position.x;
    const float dy = bestAnt->_position.y - _target_position.y;
    return std::sqrt(dx * dx + dy * dy);
}
