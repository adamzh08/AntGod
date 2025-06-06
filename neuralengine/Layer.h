//
// Created by adam on 5/31/2025.
//

#ifndef LAYER_H
#define LAYER_H

#include <functional>
#include "Activation.h"

class Layer {
public:
    int amount_neurons;
    std::function<float(float)> activation_fn;

    explicit Layer(int amount_neurons);

    Layer(int amount_neurons, float (*activation)(float));

    Layer(int amount_neurons, float (*activation_alpha)(float, float), float alpha);

    [[nodiscard]] float activation(float x) const;
};

#endif //LAYER_H
