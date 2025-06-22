//
// Created by yanam on 22.06.2025.
//

#include "ColorEditBox.h"

#include <iostream>

#include "raymath.h"
#include "../../raygui.h"

void ColorEditBox::draw() {
    auto pop = _pop.lock();
    if (!pop) {
        std::cerr << "pop is nullptr" << std::endl;
        return;
    }
    const Rectangle colorPickerBounds{
        _rect.x + 10,
        _rect.y + 10,
        _rect.width - 50,
        _rect.height - 20,
    };
    GuiColorPicker(colorPickerBounds, "Neeein", &pop->_entityColor);
}