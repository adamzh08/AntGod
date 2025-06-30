//
// Created by yanam on 18.06.2025.
//

#include "NeuroBox.h"

#include <csignal>
#include <iostream>

#include "raymath.h"
#include "../Ant.h"


class Network;

void NeuroBox::draw() {
    auto pop = _pop.lock();
    if (!pop) {
        std::cerr << "pop is nullptr" << std::endl;
        return;
    }

    if (_bestIdx != pop->_bestIdx) {
        // best has changed
        _bestIdx = pop->_bestIdx;
        if (_bestIdx != -1) {
            _network = new NEAT_Network(pop->_ants[_bestIdx]->_network);
        }
    }

    if (_bestIdx == -1) {
        DrawText("Ant not available currently!", _rect.x, _rect.y, 20, RED);
        return;
    }

    const std::vector<std::vector<Neuron> > &neurons = _network->_neurons;

    const float spaceX = _rect.width / neurons.size();
    for (int l = 0; l < neurons.size(); l++) {
        const float posX_in = _rect.x + spaceX * (l + 0.5);
        const float spaceY_in = _rect.height / neurons[l].size();

        for (int inputIdx = 0; inputIdx < neurons[l].size(); inputIdx++) {
            const float posY_in = _rect.y + spaceY_in * (inputIdx + 0.5);

            // Black = normal, Gold = bias
            DrawCircle(posX_in, posY_in, 5, l == 0 && inputIdx == 0 ? GOLD : BLACK);

            // not last layer
            if (l < neurons.size() - 1) {
                for (const Connection &connection: neurons[l][inputIdx].connections) {
                    const float posX_out = _rect.x + spaceX * (connection.l_to + 0.5);
                    const float spaceY_out = _rect.height / neurons[connection.l_to].size();
                    const float posY_out = _rect.y + spaceY_out * (connection.n_to + 0.5f);;

                    const float weight_abs = std::abs(connection.weight);
                    const float weight_line = std::clamp(weight_abs, 0.1f, 5.f) * 5;
                    DrawLineEx(
                        Vector2(posX_in, posY_in),
                        Vector2(posX_out, posY_out),
                        weight_line,
                        connection.weight > 0 ? BLUE : RED
                    );
                }
            }
        }
    }
}
