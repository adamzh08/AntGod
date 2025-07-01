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
            _rect.height / 5
        ),
        ("P(Con mut): " + std::to_string(static_cast<int>(pop->_mutation_connection_probability * 100)) + "%").c_str()
    );
    Rectangle iconRect(_rect.x + _rect.width * 0.65, _rect.y + _rect.height / 10 - _iconSize / 2, _iconSize, _iconSize);
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_connection_probability = std::max(0.f, pop->_mutation_connection_probability - _mutationProbability_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_connection_probability = std::min(1.f, pop->_mutation_connection_probability + _mutationProbability_steps);
    }


    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height * 1 / 5,
            _rect.width * 0.6,
            _rect.height / 5
        ),
        ("Str(Con mut): " + std::format("{:.2f}", pop->_mutation_connection_strength)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 5;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_connection_strength = std::max(0.f, pop->_mutation_connection_strength - _mutationStrength_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_connection_strength += _mutationStrength_steps;
    }


    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height* 2 / 5,
            _rect.width * 0.6,
            _rect.height / 5
        ),
        ("P(new con): " + std::format("{:.2f}", pop->_mutation_newConnection_probability)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 5;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_newConnection_probability = std::max(0.f, pop->_mutation_newConnection_probability - _mutationProbability_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_newConnection_probability = std::min(1.f, pop->_mutation_newConnection_probability + _mutationProbability_steps);
    }


    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height * 3 / 5,
            _rect.width * 0.6,
            _rect.height / 5
        ),
        ("P(new neuron): " + std::format("{:.2f}", pop->_mutation_newNeuron_probability)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 5;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_mutation_newNeuron_probability = std::max(0.f, pop->_mutation_newNeuron_probability - _mutationProbability_steps);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_mutation_newNeuron_probability = std::min(1.f, pop->_mutation_newNeuron_probability + _mutationProbability_steps);
    }


    GuiLabel(
        Rectangle(
            _rect.x,
            _rect.y + _rect.height * 4 / 5,
            _rect.width * 0.6,
            _rect.height / 5
        ),
        ("(next) Pop: " + std::to_string(pop->_next_ants_amount)).c_str()
    );
    iconRect.x -= _iconSize * 1.1;
    iconRect.y += _rect.height / 5;
    if (RaysEditBox::clickedMinus(iconRect)) {
        pop->_next_ants_amount = std::max(10.f, pop->_next_ants_amount * 0.9f);
    }
    iconRect.x += _iconSize * 1.1;
    if (RaysEditBox::clickedPlus(iconRect)) {
        pop->_next_ants_amount *= 1.1;
    }
}
