//
// Created by yanam on 25.06.2025.
//

#ifndef NEAT_NETWORK_H
#define NEAT_NETWORK_H
#include <functional>

struct Connection {
    int l_from, l_to; // layerIndex
    int n_from, n_to; // neuron index in layer
    float weight;
};

struct Neuron {
    float value;
    std::function<float(float)> af; // activation function
    std::vector<Connection> connections;
};

class NEAT_Network {
private:

public:
    std::vector<std::vector<Neuron> > _neurons{};

    NEAT_Network(int inputCount, int outputCount);

    std::vector<float> FeedForward(const std::vector<float> &input);

    bool TryMutateRandomConnection(float strength);

    bool TryAddRandomConnection();

    // bool TryAddRandomNeuron();
};


#endif //NEAT_NETWORK_H
