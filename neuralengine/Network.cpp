//
// Created by adam on 5/31/2025.
//
#include <iostream>
#include <fstream>
#include <functional>

#include "Network.h"

#include <assert.h>

void Network::_randomize_weights() {
    for (int layer = 0; layer < _layers.size() - 1; layer++) {
        // Xavier/Glorot initialization
        const float scale = sqrtf(2.0f / static_cast<float>(_layers[layer].amount_neurons));

        for (int i = 0; i < _layers[layer + 1].amount_neurons; i++) {
            for (int j = 0; j < _layers[layer].amount_neurons + 1; j++) {
                float r = (static_cast<float>(rand()) / RAND_MAX * 2.0f - 1.0f);
                _weights[layer][i][j] = r * scale;
            }
        }
    }
}

void Network::mutate_weights(const float mutation_probability, const float mutation_amplitude) {
    for (int layer = 0; layer < _layers.size() - 1; layer++) {
        for (int output = 0; output < _layers[layer + 1].amount_neurons; output++) {
            for (int input_idx = 0; input_idx < _layers[layer].amount_neurons + 1; input_idx++) {
                if (static_cast<float>(rand()) / RAND_MAX < mutation_probability) {
                    _weights[layer][output][input_idx] += (
                        static_cast<float>(rand()) * 2 * mutation_amplitude / RAND_MAX) - mutation_amplitude;
                }
            }
        }
    }
}

inline std::size_t hash_layer_vector(const std::vector<Layer> &layers) {
    std::size_t seed = layers.size();
    for (const auto &layer: layers) {
        // Hash the number of neurons
        seed ^= std::hash<int>{}(layer.amount_neurons) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        // Hash the function pointer if it exists
        if (layer.activation_fn) {
            // Use the target_type hash as a proxy for function identity
            seed ^= layer.activation_fn.target_type().hash_code() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    }
    return seed;
}

void Network::_save_weights(const std::string &filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Couldn't write to a file!" << std::endl;
        exit(1);
    }

    // Calculate and save architecture hash
    size_t architecture_hash = hash_layer_vector(_layers);
    file.write(reinterpret_cast<const char *>(&architecture_hash), sizeof(architecture_hash));

    // Save weights
    for (int layer = 0; layer < _layers.size() - 1; layer++) {
        for (int output = 0; output < _layers[layer + 1].amount_neurons; output++) {
            for (int input_idx = 0; input_idx < _layers[layer].amount_neurons + 1; input_idx++) {
                file.write(reinterpret_cast<const char *>(&_weights[layer][output][input_idx]), sizeof(float));
            }
        }
    }

    file.close();
}

bool Network::_load_weights(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Read and verify architecture hash
    size_t saved_hash;
    file.read(reinterpret_cast<char *>(&saved_hash), sizeof(saved_hash));

    size_t current_hash = hash_layer_vector(_layers);

    if (saved_hash != current_hash) {
        file.close();
        std::cerr << "Error: Layers don't match!" << std::endl;
        exit(1);
    }

    // Load weights
    for (int layer = 0; layer < _layers.size() - 1; layer++) {
        for (int output = 0; output < _layers[layer + 1].amount_neurons; output++) {
            for (int input_idx = 0; input_idx < _layers[layer].amount_neurons + 1; input_idx++) {
                file.read(reinterpret_cast<char *>(&_weights[layer][output][input_idx]), sizeof(float));
            }
        }
    }

    file.close();
    return true;
}

Network::Network(const std::vector<Layer> &layers) {
    _init(layers);
    _randomize_weights();
}

Network::Network(const std::vector<Layer> &layers, const std::string &filename) {
    _init(layers);

    if (!_load_weights(filename)) {
        _randomize_weights();
        _save_weights(filename);
    }
}

Network::Network(const Network &other) {
    this->_layers = other._layers;
    this->_weights = other._weights;
}

[[nodiscard]] std::vector<float> Network::feed_forward(const std::vector<float> &input) const {
    assert(input.size() == _layers[0].amount_neurons);

    std::vector<float> current_layer = input;

    for (int layer = 0; layer < _layers.size() - 1; layer++) {
        std::vector<float> next_layer(_layers[layer + 1].amount_neurons);

#pragma omp parallel for
        for (int output = 0; output < _layers[layer + 1].amount_neurons; output++) {
            next_layer[output] = _weights[layer][output][0];
            for (int input_idx = 1; input_idx < _layers[layer].amount_neurons + 1; input_idx++) {
                next_layer[output] += current_layer[input_idx - 1] * _weights[layer][output][input_idx];
            }
            next_layer[output] = _layers[layer + 1].activation(next_layer[output]);
        }

        current_layer = std::move(next_layer);
    }

    return current_layer;
}

void Network::_init(const std::vector<Layer> &layers) {
    this->_layers = layers;

    _weights.resize(_layers.size() - 1);
    for (size_t layer = 0; layer < _layers.size() - 1; layer++) {
        _weights[layer].resize(layers[layer + 1].amount_neurons);
        for (int output = 0; output < layers[layer + 1].amount_neurons; output++) {
            _weights[layer][output].resize(layers[layer].amount_neurons + 1);
        }
    }
}

void Network::free_weights() {
    _layers.clear();
    _weights.clear();
}
