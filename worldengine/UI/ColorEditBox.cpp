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

    Rectangle labelRect{
        _rect.x + _rect.width * 0.1f,
        _rect.y,
        _rect.width / 5,
        _rect.height / 3,
    };
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(RED));
    GuiLabel(labelRect, "R");

    labelRect.y += _rect.height / 3;
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(GREEN));
    GuiLabel(labelRect, "G");

    labelRect.y += _rect.height / 3;
    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLUE));
    GuiLabel(labelRect, "B");

    GuiSetStyle(LABEL, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

    float percentages[]{
        pop->_entityColor.r / 255.0f,
        pop->_entityColor.g / 255.0f,
        pop->_entityColor.b / 255.0f,
    };
    Color partialColors[]{
        Color{pop->_entityColor.r, 0, 0, 255},
        Color{0, pop->_entityColor.g, 0, 255},
        Color{0, 0, pop->_entityColor.b, 255},
    };
    Rectangle sliderBounds{
        _rect.x + _rect.width * 0.3f,
        _rect.y,
        _rect.width * 0.6f,
        _rect.height / 3,
    };
    for (int i = 0; i < 3; i++) {
        if (CheckCollisionPointCircle(GetMousePosition(), displaySlider(sliderBounds, percentages[i], 10, partialColors[i]), 10) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            _mousePressedOnIdx = i;
        }
        sliderBounds.y += _rect.height / 3;
    }
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        _mousePressedOnIdx = -1;
    }
    if (_mousePressedOnIdx != -1) {
        float p = (GetMousePosition().x - sliderBounds.x) / sliderBounds.width;
        p = Clamp(p, 0.0f, 1.0f);
        percentages[_mousePressedOnIdx] = p;
    }
    pop->_entityColor = Color{
        static_cast<unsigned char>(percentages[0] * 255),
        static_cast<unsigned char>(percentages[1] * 255),
        static_cast<unsigned char>(percentages[2] * 255),
        255,
    };
}

Vector2 ColorEditBox::displaySlider(Rectangle bounds, float percentage, float radius, Color color) {
    // smaller rect in center
    const Rectangle sliderBarRect{
        bounds.x + bounds.width * 0.1f,
        bounds.y + bounds.height * 0.3f,
        bounds.width * 0.8f,
        bounds.height * 0.4f,
    };
    DrawLineEx(
        Vector2(sliderBarRect.x, sliderBarRect.y + sliderBarRect.height / 2),
        Vector2(sliderBarRect.x + sliderBarRect.width, sliderBarRect.y + sliderBarRect.height / 2),
        10,
        Fade(color, 0.5f)
    );
    const Vector2 circlePos{
        sliderBarRect.x + sliderBarRect.width * percentage,
        sliderBarRect.y + sliderBarRect.height / 2.f,
    };
    DrawCircle(circlePos.x, circlePos.y, radius, color);
    return circlePos;
}
