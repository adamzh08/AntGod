//
// Created by yanam on 18.06.2025.
//

#ifndef NEUROBOX_H
#define NEUROBOX_H
#include <memory>
#include <mutex>

#include "InfoBox.h"


class Ant;

class NeuroBox : public InfoBox {
private:
    int _bestIdx = -1;
    NEAT_Network* _network = nullptr;
public:
    NeuroBox(const Rectangle rect, std::shared_ptr<Population> pop): InfoBox(rect, pop) {
    };
    void draw() override;
};


#endif //NEUROBOX_H
