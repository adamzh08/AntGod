//
// Created by yanam on 18.06.2025.
//

#include "NeuroBox.h"

#include <csignal>
#include <iostream>

#include "Ant.h"


class Network;

void NeuroBox::draw() const {
    std::signal(SIGSEGV, [](const int i) {
        std::cout << i << std::endl;
    });


    if (const auto antShared = _ant.lock()) {  // lock weak_ptr to shared_ptr
        // use antShared safely
        const Network& network = antShared->_network;

        const float spaceX = _rect.width / network._layers.size();
        for (int l = 0; l < network._layers.size(); l++) {
            const float posX_in = _rect.x + spaceX * (l + 0.5);
            const float posX_out = _rect.x + spaceX * (l + 1.5);

            const float spaceY_in = _rect.height / network._layers[l].amount_neurons;
            const float spaceY_out = l < network._layers.size() - 1
                                         ? _rect.height / network._layers[l + 1].amount_neurons
                                         : -1; // doesnt matter here

            for (int inputIdx = 0; inputIdx < network._layers[l].amount_neurons; inputIdx++) {
                const float posY_in = _rect.y + spaceY_in * (inputIdx + 0.5);

                // input neuron
                DrawCircle(posX_in, posY_in, 3, _color);

                // not last layer
                if (l < network._layers.size() - 1) {
                    for (int outPutIdx = 0; outPutIdx < network._layers[l + 1].amount_neurons; outPutIdx++) {
                        const float posY_out = _rect.y + spaceY_out * (outPutIdx + 0.5);

                        const float weight = network._weights[l][outPutIdx][inputIdx + 1];
                        const float weight_abs = std::abs(weight);
                        const float weight_clamped = std::clamp(weight_abs, 1.f, 20.f);
                        DrawLineEx(
                            Vector2(posX_in, posY_in),
                            Vector2(posX_out, posY_out),
                            weight_clamped,
                            weight == weight_abs ? BLUE : RED
                        );
                    }
                }
            }
        }
        drawBounds();
    } else {
        DrawText("Colony went extinct or ant is gone!", _rect.x, _rect.y, 20, BLACK);
    }
}

void NeuroBox::setEntity(std::shared_ptr<Ant> ant) {
    _ant = ant;
}
