//
// Created by yanam on 19.06.2025.
//

#include "RaysEditBox.h"

#include <iostream>
#include <string>

#include "TextureCollection.h"
#include "../../raygui.h"
#include "../Lines.h"


void RaysEditBox::draw() {
    auto pop = _pop.lock();
    if (!pop) {
        std::cerr << "pop is nullptr" << std::endl;
        return;
    }
    GuiLabel(
        {
            _rect.x,
            _rect.y,
            _rect.width / 2,
            _rect.height / 2,
        },
        ("Ray length: " + std::to_string(pop->_rays_radius)).c_str()
    );
    Rectangle iconRect(_rect.x + _rect.width * 0.65, _rect.y + _rect.height / 4 - _iconSize / 2, _iconSize, _iconSize);
    if (clickedMinus(iconRect)) {
        pop->_rays_radius = std::max(_length_min, pop->_rays_radius - _length_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (clickedPlus(iconRect)) {
        pop->_rays_radius = std::min(_length_max, pop->_rays_radius + _length_steps);
    }
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height / 2,
            _rect.width * 0.6,
            _rect.height / 2
        ),
        ("Field of view: " + std::to_string(static_cast<int>(pop->_rays_fov * RAD2DEG)) + "°").c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 2;
    if (clickedMinus(iconRect)) {
        pop->_rays_fov = std::max(0.f, pop->_rays_fov - _fov_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (clickedPlus(iconRect)) {
        pop->_rays_fov += _fov_steps;
    }
}

bool RaysEditBox::clickedMinus(const Rectangle rect) {
    const bool shouldHighLight = CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(GRAY, shouldHighLight? 0.8 : 0.5));
    DrawRectangleLinesEx(rect, 3, BLACK);

    DrawLineEx(
        Vector2(rect.x + rect.width * 0.2, rect.y + rect.height / 2),
        Vector2(rect.x + rect.width * 0.8, rect.y + rect.height / 2),
        5,
        Fade(GRAY, 0.5f)
    );
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rect);
}
bool RaysEditBox::clickedPlus(const Rectangle rect) {
    const bool shouldHighLight = CheckCollisionPointRec(GetMousePosition(), rect) && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

    DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(GRAY, shouldHighLight? 0.8 : 0.5));
    DrawRectangleLinesEx(rect, 3, BLACK);

    DrawLineEx(
        Vector2(rect.x + rect.width * 0.2, rect.y + rect.height / 2),
        Vector2(rect.x + rect.width * 0.8, rect.y + rect.height / 2),
        5,
        Fade(GRAY, 0.5f)
    );
    DrawLineEx(
        Vector2(rect.x + rect.width / 2, rect.y + rect.height * 0.2),
        Vector2(rect.x + rect.width / 2, rect.y + rect.height * 0.8),
        5,
        Fade(GRAY, 0.5f)
    );
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rect);
}

