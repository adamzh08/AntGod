//
// Created by yanam on 18.06.2025.
//

#ifndef INFORECT_H
#define INFORECT_H
#include "raylib.h"


class InfoBox {
protected:
    const Rectangle _rect;
    const Color _color;

public:
    explicit InfoBox(const Rectangle rect, const Color color): _rect(rect), _color(color) {};

    ~InfoBox() {};

    virtual void draw() const = 0;

    void drawBounds() const {
        DrawRectangleLinesEx(_rect, 2, _color);
    }
};



#endif //INFORECT_H
