//
// Created by yanam on 18.06.2025.
//

#ifndef INFORECT_H
#define INFORECT_H
#include <memory>

#include "raylib.h"

#include "../Population.h"

class InfoBox {
protected:
    const Rectangle _rect;
    std::weak_ptr<Population> _pop;

public:
    explicit InfoBox(const Rectangle rect, std::shared_ptr<Population> pop): _rect(rect), _pop(pop) {};

    ~InfoBox() = default;

    virtual void draw() = 0;
    void drawBounds() const {
        if (auto pop = _pop.lock()) {
            DrawRectangleLinesEx(_rect, 5, pop->_entityColor);
        }
    }
};



#endif //INFORECT_H
