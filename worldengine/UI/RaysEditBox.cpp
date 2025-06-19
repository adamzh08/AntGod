//
// Created by yanam on 19.06.2025.
//

#include "RaysEditBox.h"

#include <iostream>

#include "../TextureCollection.h"
#include "../../raygui.h"


void RaysEditBox::draw() const {
    GuiLabel(
        {
            _rect.x,
            _rect.y,
            _rect.width / 2,
            _rect.height / 2,
        },
        ("Ray length: " + std::to_string(_pop._rays_radius)).c_str()
    );
    if (clickedMinus({_rect.x + _rect.width / 2, _rect.y, 50, 50})) {
        _pop._rays_radius = std::max(5, _pop._rays_radius - _length_steps);
    }
    if (clickedPlus({_rect.x + _rect.width / 2 + 60, _rect.y, 50, 50})) {
        _pop._rays_radius += _length_steps;
    }
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height / 2,
            _rect.width * 0.6,
            _rect.height / 2
        ),
        ("Field of view: " + std::to_string(static_cast<int>(_pop._rays_fov * RAD2DEG)) + "°").c_str()
    );

    if (clickedMinus(Rectangle(_rect.x + _rect.width * 0.6, _rect.y + _rect.height / 2, 50, 50))) {
        _pop._rays_fov = std::max(0.f, _pop._rays_fov - _fov_steps);
    }
    if (clickedPlus(Rectangle(_rect.x + _rect.width * 0.6 + 60, _rect.y + _rect.height / 2, 50, 50))) {
        _pop._rays_fov += _fov_steps;
    }
    drawBounds();
}

bool RaysEditBox::clickedMinus(const Rectangle rect) {
    DrawRectangleLinesEx(rect, 3, BLACK);
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(GRAY, 0.5f));

    DrawLineEx(
        Vector2(rect.x + rect.width * 0.2, rect.y + rect.height / 2),
        Vector2(rect.x + rect.width * 0.8, rect.y + rect.height / 2),
        5,
        Fade(GRAY, 0.5f)
    );
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), rect);
}
bool RaysEditBox::clickedPlus(const Rectangle rect) {
    DrawRectangleLinesEx(rect, 3, BLACK);
    DrawRectangle(rect.x, rect.y, rect.width, rect.height, Fade(GRAY, 0.5f));

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

