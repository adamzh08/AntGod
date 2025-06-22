//
// Created by yanam on 19.06.2025.
//

#ifndef RAYSEDITBOX_H
#define RAYSEDITBOX_H
#include "InfoBox.h"
#include "../Population.h"


class RaysEditBox : public InfoBox {
    float _iconSize;

    float _length_steps = 5;
    float _length_min = 50;
    float _length_max = 500;
    float _fov_steps = 5 * DEG2RAD;

public:
    RaysEditBox(std::shared_ptr<Population> pop, const Rectangle rect): InfoBox(rect, pop), _iconSize(std::min(46.f, rect.height * 0.4f)) {
    }

    static bool clickedMinus(Rectangle rect);
    static bool clickedPlus(Rectangle rect);

    void draw() override;
};


#endif //RAYSEDITBOX_H
