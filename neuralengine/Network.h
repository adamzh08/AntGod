//
// Created by adam on 5/31/2025.
//

#ifndef NETWORK_H
#define NETWORK_H

#include <array>
#include <iomanip>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

#include "Layer.h"

class Network {
private:

    void _randomize_weights();

    void _save_weights(const std::string &filename) const;

    bool _load_weights(const std::string &filename);

    void _init(const std::vector<Layer> &layers);

public:
    std::vector<Layer> _layers;
    std::vector<std::vector<std::vector<float> > > _weights;

    explicit Network(const std::vector<Layer> &layers);

    Network(const std::vector<Layer> &layers, const std::string &filename);

    Network(const Network &other);

    void mutate_weights(float mutation_probability, float mutation_amplitude);

    [[nodiscard]] std::vector<float> feed_forward(const std::vector<float> &input) const;

    void free_weights();

    void replaceWeightsWithOther(const Network &other, double p);
};

#endif //NETWORK_H
