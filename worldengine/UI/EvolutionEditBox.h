//
// Created by yanam on 19.06.2025.
//

#ifndef EVOLUTIONEDITBOX_H
#define EVOLUTIONEDITBOX_H
#include "InfoBox.h"
#include "../Population.h"


class EvolutionEditBox : public InfoBox {
private:
    float _iconSize;

    float _mutationP_steps = 0.05;
    float _mutationS_steps = 0.05;
    int _antCount_steps = 10;

public:
    EvolutionEditBox(std::shared_ptr<Population> pop, const Rectangle rect): InfoBox(rect, pop), _iconSize(std::min(46.f, rect.height * 0.3f)) {
    }

    void draw() override;
};


#endif //EVOLUTIONEDITBOX_H
