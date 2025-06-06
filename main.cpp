#include <iostream>
#include <vector>

#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main() {
    srand(time(nullptr));

    std::vector<Layer> layers = {
        Layer(3),
        Layer(2048, Activation::gelu),
        Layer(2048, Activation::gelu),
        Layer(2, Activation::gelu),
    };

    Network net = Network(layers, "weights.bin");

    clock_t start = clock();
    float sum = 0;
    for (int i = 0; i < 100; i++) {
        std::vector<float> input = {i/100.0f, 0.2, 0.3};
        std::vector<float> output = net.feed_forward(input);
        sum += output[0];
    }
    std::cout << sum << std::endl;
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    std::cout << elapsed << std::endl;

    return 0;
}
