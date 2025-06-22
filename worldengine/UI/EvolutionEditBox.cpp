//
// Created by yanam on 19.06.2025.
//

#include "EvolutionEditBox.h"

#include <format>
#include <iostream>

#include "RaysEditBox.h"
#include "../../raygui.h"


void EvolutionEditBox::draw() {
    auto pop = _pop.lock();
    if (!pop) {
        std::cerr << "pop is nullptr" << std::endl;
        return;
    }

    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y,
            _rect.width * 0.6,
            _rect.height / 3
        ),
        ("P(Mutation): " + std::to_string(static_cast<int>(pop->_mutation_probability * 100)) + "%").c_str()
    );
    Rectangle iconRect(_rect.x + _rect.width * 0.65, _rect.y + _rect.height / 6 - _iconSize / 2, _iconSize, _iconSize);
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_probability = std::max(0.f, pop->_mutation_probability - _mutationP_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_probability = std::min(1.f, pop->_mutation_probability + _mutationP_steps);
    }
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height / 3,
            _rect.width * 0.6,
            _rect.height / 3
        ),
        ("Strength(Mut): " + std::format("{:.2f}", pop->_mutation_strength)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 3;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_strength = std::max(0.f, pop->_mutation_strength - _mutationS_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_strength += _mutationS_steps;
    }
    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height * 2 / 3,
            _rect.width * 0.6,
            _rect.height / 3
        ),
        ("Population: " + std::to_string(pop->_ants_amount)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 3;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_ants_amount = std::max(10.f, pop->_ants_amount * 0.9f);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_ants_amount *= 1.1;
    }
}
