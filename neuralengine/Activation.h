//
// Created by adam on 6/1/2025.
//

#ifndef ACTIVATION_H
#define ACTIVATION_H

#include <cmath>

#define M_PI 3.14159265358979323846

class Activation {
public:
    static float sigmoid(float x);
    static float tanh(float x);
    static float relu(float x);
    static float prelu(float x, float alpha);
    static float elu(float x, float alpha);
    static float gelu(float x);
};

#endif //ACTIVATION_H
