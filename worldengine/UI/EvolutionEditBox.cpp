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
    if (RaysEditBox::clickedMinus(Rectangle(_rect.x + _rect.width * 0.6, _rect.y, 50, 50))) {
        _pop._mutation_probability = std::max(0.f, _pop._mutation_probability - _mutationP_steps);
    }
    if (RaysEditBox::clickedPlus(Rectangle(_rect.x + _rect.width * 0.6 + 60, _rect.y, 50, 50))) {
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
    if (RaysEditBox::clickedMinus(Rectangle(_rect.x + _rect.width * 0.6, _rect.y + _rect.height / 2, 50, 50))) {
        _pop._mutation_strength = std::max(0.f, _pop._mutation_strength - _mutationS_steps);
    }
    if (RaysEditBox::clickedPlus(Rectangle(_rect.x + _rect.width * 0.6 + 60, _rect.y + _rect.height / 2, 50, 50))) {
        _pop._mutation_strength += _mutationS_steps;
    }
    drawBounds();
}
