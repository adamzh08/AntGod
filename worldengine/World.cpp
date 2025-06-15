//
// Created by yanam on 09.06.2025.
//

#include <iostream>

#include "World.h"

#define RAYGUI_IMPLEMENTATION
#include "../raygui.h"


World &World::setLines(const Lines &lines) {
    _lines = lines;
    return *this;
}

World &World::setPopulations(std::vector<Population> &&populations) {
    _populations = std::move(populations);
    return *this;
}

World &World::setGenerationDuration(int duration) {
    _generation_frameDuration = duration;
    return *this;
}

int frameCounter = 0;

void World::act() {
    frameCounter++;

    if (frameCounter == _generation_frameDuration) {
        frameCounter = 0;
        for (Population &population: _populations) {
            population.flood();
        }
    }
    for (Population &population: _populations) {
        population.act();
    }
}

void World::draw() {
    drawGame();
    drawButtons();
    drawUserInfo();
}

void World::drawGame() const {
    // borders
    for (const Line &line: _lines._lines) {
        DrawLineEx(line.start, line.end, 1.5, BLACK);
    }

    // colonies
    for (const Population &population: _populations) {
        population.draw();
    }

    // new wall that is being drawn
    if (_drawVar_borderStartPos.has_value()) {
        DrawLineV(_drawVar_borderStartPos.value(), GetMousePosition(), BLUE);
    }
}

void World::drawUserInfo() const {
    std::ostringstream totalInfo;

    totalInfo << "Your total info" << std::endl;
    totalInfo << std::endl;
    totalInfo << "user mode: " << _userMode << std::endl;
    totalInfo << "frames left: " << _generation_frameDuration - frameCounter << std::endl;

    GuiTextBox(
        Rectangle(GetScreenWidth() - 200.f, 0, GetScreenWidth(), GetScreenHeight()),
        const_cast<char *>(totalInfo.str().c_str()),
        50,
        false
    );
}

void World::drawButtons() {
    // switch to draw mode button
    if (GuiButton(Rectangle(0, GetScreenHeight() - 60, 200, 50), "switch to draw mode")) {
        _userMode = DRAWING;
    }

    // switch to normal mode button
    if (GuiButton(Rectangle(230, GetScreenHeight() - 60, 200, 50), "switch to normal mode")) {
        _userMode = JUST_LOOKING;
    }
}

void World::handleUserInput() {
    switch (_userMode) {
        case JUST_LOOKING: {
            break;
        }
        case DRAWING: {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                _drawVar_borderStartPos = GetMousePosition();
            } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                _lines._lines.push_back(Line(_drawVar_borderStartPos.value(), GetMousePosition()));
                _drawVar_borderStartPos.reset();
            }
            break;
        }
        case MOVE_OBJECTS: {
            // Todo
            break;
        }
    }
}
