//
// Created by yanam on 09.06.2025.
//

#include <iostream>
#include <string>

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
    // the bottom game boundary
    DrawLineEx(
        Vector2(0, GetScreenHeight() - 100),
        Vector2(GetScreenWidth() - 300, GetScreenHeight() - 100),
        5,
        BLACK
    );
    // the right game boundary
    DrawLineEx(
        Vector2(GetScreenWidth() - 300, 0),
        Vector2(GetScreenWidth() - 300, GetScreenHeight() - 100),
        5,
        BLACK
    );

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
    drawLineOfText(("user mode: " + std::string(strFromUserMode())).c_str(), 0);
    drawLineOfText(("frames left: " + std::to_string(_generation_frameDuration - frameCounter)).c_str(), 1);
}


void World::drawLineOfText(const char *line, int idx) const {
    GuiDrawText(
        line,
        Rectangle(
            GetScreenWidth() - 290,
            10 + idx * GuiGetStyle(DEFAULT, TEXT_SIZE),
            190,
            10
        ),
        TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP,
        BLACK
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


char *World::strFromUserMode() const {
    switch (_userMode) {
        case JUST_LOOKING: return "normal";
        case DRAWING: return "drawing";
        case MOVE_OBJECTS: return "move";
        default: return "?";
    }
}
