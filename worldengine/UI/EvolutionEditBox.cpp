//
// Created by yanam on 19.06.2025.
//

#include "EvolutionEditBox.h"

#include <format>

#include "RaysEditBox.h"
#include "../../raygui.h"


void EvolutionEditBox::draw() const {
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y,
            _rect.width * 0.6,
            _rect.height / 2
        ),
        ("P(Mutation): " + std::to_string(static_cast<int>(_pop._mutation_probability * 100)) + "%").c_str()
    );
    Rectangle iconRect(_rect.x + _rect.width * 0.6, _rect.y + _rect.height / 4 - _iconSize / 2, _iconSize, _iconSize);
    if (RaysEditBox::clickedMinus(iconRect)) {
        _pop._mutation_probability = std::max(0.f, _pop._mutation_probability - _mutationP_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        _pop._mutation_probability = std::min(1.f, _pop._mutation_probability + _mutationP_steps);
    }
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height / 2,
            _rect.width * 0.6,
            _rect.height / 2
        ),
        ("Strength(Mut): " + std::format("{:.2f}", _pop._mutation_strength)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 2;
    if (RaysEditBox::clickedMinus(iconRect)) {
        _pop._mutation_strength = std::max(0.f, _pop._mutation_strength - _mutationS_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        _pop._mutation_strength += _mutationS_steps;
    }
    drawBounds();
}
