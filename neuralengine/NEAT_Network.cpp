//
// Created by yanam on 25.06.2025.
//

#include "NEAT_Network.h"

#include <cassert>
#include <iostream>

#include "Activation.h"

NEAT_Network::NEAT_Network(const int inputCount, const int outputCount) {
    // bias already inserted
    std::vector<Neuron> inputLayer{
        {
            0,
            Activation::identity,
            {}
        }
    };
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
            for (const Connection &connection: neuron.connections) {
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
        connections[rand() % connections.size()].weight += strength * RandomWeight();
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
            RandomWeight()
        });
        return true; // successful
    }
    return false; // failed
}

bool NEAT_Network::TryAddRandomNeuron() {
    const int maxTries = 10; // to avoid being stuck in while loop

    for (int tryCount = 0; tryCount < maxTries; tryCount++) {
        const int l_from = rand() % (_neurons.size() - 1); // exclude output layer
        const int n_from = rand() % _neurons[l_from].size();

        std::vector<Connection> &connections = _neurons[l_from][n_from].connections;
        if (connections.empty()) continue;
        const int connectionIdx = rand() % connections.size();

        int insertionLayerIdx = (l_from + connections[connectionIdx].l_to) / 2;

        if (l_from == insertionLayerIdx) {
            // the connection goes from one layer directly to the next
            // we have to create a new layer and insert it inbetween
            insertionLayerIdx++;
            std::vector<Neuron> newLayer{
                Neuron{
                    0,
                    Activation::tanh,
                    {
                        {
                            insertionLayerIdx,
                            insertionLayerIdx,
                            0,
                            connections[connectionIdx].n_to,
                            RandomWeight()
                        }
                    }
                }
            };
            _neurons.insert(_neurons.begin() + insertionLayerIdx, newLayer);

            // shift all the connections one to the right
            for (auto &layer : _neurons) {
                for (auto &neuron : layer) {
                    for (auto &conn : neuron.connections) {
                        if (conn.l_to >= insertionLayerIdx) {
                            conn.l_to++;
                        }
                    }
                }
            }
        } else {
            // add a new neuron to a layer in the middle
            _neurons[insertionLayerIdx].push_back(Neuron{
                0,
                Activation::tanh,
                {
                    Connection{
                        insertionLayerIdx,
                        connections[connectionIdx].l_to,
                        static_cast<int>(_neurons[insertionLayerIdx].size()),
                        connections[connectionIdx].n_to,
                        RandomWeight()
                    }
                }
            });
        }
        // redirect old connection
        connections[connectionIdx].l_to = insertionLayerIdx;
        connections[connectionIdx].n_to = _neurons[insertionLayerIdx].size() - 1;

        return true; // successful
    }
    return false; // failed
}

float NEAT_Network::RandomWeight() {
    return 2 * static_cast<float>(rand()) / RAND_MAX - 1;
}
