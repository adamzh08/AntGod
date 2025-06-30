//
// Created by yanam on 25.06.2025.
//

#include "NEAT_Network.h"

#include <cassert>
#include <iostream>

#include "Activation.h"

NEAT_Network::NEAT_Network(const int inputCount, const int outputCount) {
    // bias already inserted
    std::vector<Neuron> inputLayer{{
        0,
        Activation::identity,
        {}
    }};
    std::vector<Neuron> outputLayer{};

    for (int i = 0; i < inputCount; i++) {
        inputLayer.push_back({
            0,
            Activation::identity,
            {}
        });
    }
    for (int i = 0; i < outputCount; i++) {
        outputLayer.push_back({
            0,
            Activation::tanh,
            {}
        });
    }
    _neurons.push_back(inputLayer);
    _neurons.push_back(outputLayer);
}


// takes a reference because input vector is not modified
std::vector<float> NEAT_Network::FeedForward(const std::vector<float> &input) {
    assert(input.size() == _neurons[0].size() - 1); // -1 for bias

    _neurons[0][0].value = 1; // bias
    for (int i = 0; i < input.size(); i++) {
        _neurons[0][i + 1].value = input[i];
    }

    for (int l = 0; l < _neurons.size(); l++) {
        // apply activation function
        for (Neuron &neuron: _neurons[l]) {
            neuron.value = neuron.af(neuron.value);
        }
        // forward pass
        for (Neuron &neuron: _neurons[l]) {
            for (const Connection& connection: neuron.connections) {
                _neurons[connection.l_to][connection.n_to].value += neuron.value * connection.weight;
            }
        }
        // reset if not output layer
        if (l != _neurons.size() - 1) {
            for (Neuron &neuron: _neurons[l]) {
                neuron.value = 0;
            }
        }
    }

    std::vector<float> output;
    for (Neuron &outputNeuron: _neurons.back()) {
        output.push_back(outputNeuron.value);
        outputNeuron.value = 0;
    }
    return output;
}

bool NEAT_Network::TryMutateRandomConnection(const float strength) {
    const int maxTries = 10; // to avoid being stuck in while loop

    for (int tryCount = 0; tryCount < maxTries; tryCount++) {
        const int l = rand() % _neurons.size();
        const int n_from = rand() % _neurons[l].size(); // chose a random neuron in the layer

        auto &connections = _neurons[l][n_from].connections;
        if (connections.empty()) continue; // neuron has no outgoing connections


        // mutate a random connection
        connections[rand() % connections.size()].weight += strength * (2 * static_cast<float>(rand()) / RAND_MAX - 1);
        return true; // successful
    }
    return false; // failed
}

bool NEAT_Network::TryAddRandomConnection() {
    const int maxTries = 10; // to avoid being stuck in while loop

    for (int tryCount = 0; tryCount < maxTries; tryCount++) {
        const int l_from = rand() % (_neurons.size() - 1); // exclude output layer
        const int n_from = rand() % _neurons[l_from].size();

        const int l_to = l_from + 1 + rand() % (_neurons.size() - (l_from + 1));
        const int n_to = rand() % _neurons[l_to].size();

        // verify there isn't already a connection between the 2
        bool connectionBetween = false;
        for (const Connection &connection: _neurons[l_from][n_from].connections) {
            if (connection.l_to == l_to && connection.n_to == n_to) {
                connectionBetween = true;
                break;
            }
        }
        if (connectionBetween) continue; // connection already there

        _neurons[l_from][n_from].connections.push_back({
            l_from,
            l_to,
            n_from,
            n_to,
            2 * static_cast<float>(rand()) / RAND_MAX - 1
        });
        return true; // successful
    }
    return false; // failed
}

/*
bool NEAT_Network::TryAddRandomNeuron() {
    const int maxTries = 10; // to avoid being stuck in while loop

    for (int tryCount = 0; tryCount < maxTries; tryCount++) {
        const int randLayerIdx = rand() % (_layers.size() - 1); // exclude output layer
        const int randNeuronIdx = rand() % _layers[randLayerIdx].neurons.size(); // chose a random neuron in the layer

        // make sure the chosen neuron has outgoing connections
        if (_layers[randLayerIdx].neurons[randNeuronIdx].connections.empty()) continue;

        // chose a random neuron in the next layer
        const int randConnectionIdx = rand() % _layers[randLayerIdx].neurons[randNeuronIdx].connections.size();

        Neuron newNeuron{
            0,
            Activation::tanh,
            {
                // new layer -> 2nd chosen layer
                {
                    _layers[randLayerIdx].neurons[randNeuronIdx].connections[randConnectionIdx].first,
                    2 * static_cast<float>(rand()) / RAND_MAX - 1
                }
            }
        };

        // create a new layer consisting of only 1 neuron
        Layer newLayer{
            randLayerIdx + 1,
            &_layers[randLayerIdx + 1],
            {
                newNeuron
            },
        };
        _layers.insert(_layers.begin() + randLayerIdx, newLayer);

        // 1st chosen layer -> new layer
        auto &connections = _layers[randLayerIdx].neurons[randNeuronIdx].connections;
        connections.erase(connections.begin() + randConnectionIdx);
        connections.push_back({
            0,
            2 * static_cast<float>(rand()) / RAND_MAX - 1
        });
        return true; // successful
    }
    return false; // failed
}
*/
