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
    std::vector<Layer> layers;
    size_t layers_amount;

    std::vector<std::vector<std::vector<float>>> weights;

    void randomize_weights();

    void save_weights(const std::string& filename) const;
    bool load_weights(const std::string& filename);

    void init(const std::vector<Layer> &layers);
public:
    explicit Network(const std::vector<Layer> &layers);
    Network(const std::vector<Layer> &layers, const std::string& filename);
    Network(const Network &other) noexcept;

    void mutate_weights(float mutation_probability, float mutation_amplitude);

    [[nodiscard]] std::vector<float> feed_forward(const std::vector<float>& input) const;

    void free_weights();
};

#endif //NETWORK_H
