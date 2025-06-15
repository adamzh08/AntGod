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

World &World::setLayout(int space_right, int space_bottom) {
    _space_right = space_right;
    _space_bottom = space_bottom;
    return *this;
}

void World::act() {
    _frameCount++;

    if (_frameCount == _generation_frameDuration) {
        for (Population &population: _populations) {
            population.flood();
        }
        _generation_count++;
        _frameCount = 0;
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
        Vector2(0, GetScreenHeight() - _space_bottom),
        Vector2(GetScreenWidth() - _space_right, GetScreenHeight() - _space_bottom),
        5,
        BLACK
    );
    // the right game boundary
    DrawLineEx(
        Vector2(GetScreenWidth() - _space_right, 0),
        Vector2(GetScreenWidth() - _space_right, GetScreenHeight() - _space_bottom),
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
    GuiSetStyle(DEFAULT, TEXT_SIZE, 30);

    drawLineOfText(
        ("user mode: " + std::string(strFromUserMode())).c_str(),
        0
    );
    drawLineOfText(
        ("frames left: " + std::to_string(_generation_frameDuration - _frameCount)).c_str(),
        1
    );
    drawLineOfText(
        ("gen count: " + std::to_string(_generation_count)).c_str(),
        2
    );
}


void World::drawLineOfText(const char *line, int idx) const {
    GuiDrawText(
        line,
        Rectangle(
            GetScreenWidth() - (_space_right - 10),
            10 + idx * GuiGetStyle(DEFAULT, TEXT_SIZE),
            _space_right - 10,
            10
        ),
        TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP,
        BLACK
    );
}

void World::drawButtons() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // switch to draw mode button
    if (GuiButton(Rectangle(0, GetScreenHeight() - 60, 225, 50), "switch to draw mode")) {
        _userMode = DRAWING;
    }

    // switch to normal mode button
    if (GuiButton(Rectangle(250, GetScreenHeight() - 60, 225, 50), "switch to observe mode")) {
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
        case JUST_LOOKING: return "observe";
        case DRAWING: return "draw";
        case MOVE_OBJECTS: return "move";
        default: return "?";
    }
}
