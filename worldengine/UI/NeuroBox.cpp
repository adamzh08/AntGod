//
// Created by yanam on 18.06.2025.
//

#include "NeuroBox.h"

#include <csignal>
#include <iostream>

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
            _network = new Network(pop->_ants[_bestIdx]->_network);
        }
    }

    if (_bestIdx == -1) {
        DrawText("Ant not available currently!", _rect.x, _rect.y, 20, RED);
        return;
    }

    const float spaceX = _rect.width / _network->_layers.size();
    for (int l = 0; l < _network->_layers.size(); l++) {
        const float posX_in = _rect.x + spaceX * (l + 0.5);
        const float posX_out = _rect.x + spaceX * (l + 1.5);

        const float spaceY_in = _rect.height / _network->_layers[l].amount_neurons;
        const float spaceY_out = l < _network->_layers.size() - 1
                                     ? _rect.height / _network->_layers[l + 1].amount_neurons
                                     : -1; // doesnt matter here

        for (int inputIdx = 0; inputIdx < _network->_layers[l].amount_neurons; inputIdx++) {
            const float posY_in = _rect.y + spaceY_in * (inputIdx + 0.5);

            // input neuron
            DrawCircle(posX_in, posY_in, 3, BLACK);

            // not last layer
            if (l < _network->_layers.size() - 1) {
                for (int outPutIdx = 0; outPutIdx < _network->_layers[l + 1].amount_neurons; outPutIdx++) {
                    const float posY_out = _rect.y + spaceY_out * (outPutIdx + 0.5);

                    const float weight = _network->_weights[l][outPutIdx][inputIdx + 1];
                    const float weight_abs = std::abs(weight);
                    const float weight_line = std::clamp(weight_abs, 0.1f, 5.f) * 5;
                    DrawLineEx(
                        Vector2(posX_in, posY_in),
                        Vector2(posX_out, posY_out),
                        weight_line,
                        weight == weight_abs ? BLUE : RED
                    );
                }
            }
        }
    }
}
