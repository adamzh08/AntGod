//
// Created by adam on 6/1/2025.
//

#include "Activation.h"

#define M_PI 3.1415926535897932384626433832795

float Activation::sigmoid(const float x) {
    return 1.0 / (1.0 + std::exp(-x));
}

float Activation::tanh(float x) {
    return std::tanh(x);
}

float Activation::relu(const float x) {
    return x > 0 ? x : 0;
}

float Activation::prelu(const float x, const float alpha) {
    return x > 0 ? x : x * alpha;
}

float Activation::elu(float x, float alpha) {
    return x > 0.0 ? x : alpha * (std::exp(x) - 1.0);
}

float Activation::gelu(float x) {
    return 0.5 * x * (1.0 + std::tanh(std::sqrt(2.0 / M_PI) * (x + 0.044715 * x * x * x)));
}
