//
// Created by adam on 6/1/2025.
//

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>
#define M_PI 3.14159265358979323846

namespace Activation {
    inline float sigmoid(const float x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    inline float tanh(float x) {
        return std::tanh(x);
    }

    inline float relu(const float x) {
        return x > 0 ? x : 0;
    }

    inline float prelu(const float x, const float alpha) {
        return x > 0 ? x : x * alpha;
    }

    inline float elu(float x, float alpha) {
        return x > 0.0 ? x : alpha * (std::exp(x) - 1.0);
    }

    inline float gelu(float x) {
        return 0.5 * x * (1.0 + std::tanh(std::sqrt(2.0 / M_PI) * (x + 0.044715 * x * x * x)));
    }
}

#endif //ACTIVATION_H
