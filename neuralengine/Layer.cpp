//
// Created by adam on 5/31/2025.
//

#include "Layer.h"

Layer::Layer(const int amount_neurons) {
    this->amount_neurons = amount_neurons;
    this->activation_fn = nullptr;
}

Layer::Layer(const int amount_neurons, float (*activation)(float)) {
    this->amount_neurons = amount_neurons;
    this->activation_fn = activation;
}

Layer::Layer(const int amount_neurons, float (*activation_alpha)(float, float), float alpha) {
    this->amount_neurons = amount_neurons;
    this->activation_fn = [activation_alpha, alpha](const float x) -> float {
        return activation_alpha(x, alpha);
    };
}

[[nodiscard]] float Layer::activation(const float x) const {
    if (activation_fn) {
        return activation_fn(x);
    }
    return x; // Identity function if no activation set
}