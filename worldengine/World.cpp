//
// Created by yanam on 09.06.2025.
//

#include <iostream>
#include <string>

#include "World.h"

#define RAYGUI_IMPLEMENTATION
#include <cfloat>

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
    if (!_paused) {
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
}

void World::draw() {
    drawGame();
    handleUserInput();
    drawUserInfo();
}

void World::drawGame() {
    // obstacles
    _lines.draw();

    // colonies
    for (Population &population: _populations) {
        population.draw();
    }
}

void World::handleUserInput() {
    handleMouseClicks();
    handleButtons();
}


void World::handleButtons() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

    // switch to normal mode button
    if (GuiButton(Rectangle(0, GetScreenHeight() - 60, 100, 50), "Observe")) {
        _userMode = OBSERVE;
    }
    // switch to draw mode button
    if (GuiButton(Rectangle(125, GetScreenHeight() - 60, 100, 50), "Draw")) {
        _userMode = DRAWING;
    }
    // switch to normal mode button
    if (GuiButton(Rectangle(250, GetScreenHeight() - 60, 100, 50), "Move")) {
        _userMode = MOVE_OBJECTS;
    }

    // info for RayGui icons: in raygui.h, look at enum 'GuiIconName' with ctrl + f to find the icon ids fast
    // pause button
    if (GuiButton(Rectangle(375, GetScreenHeight() - 60, 50, 50), _paused ? "#131#" : "#132#")) {
        _paused = !_paused;
    }
    if (GuiButton(Rectangle(450, GetScreenHeight() - 60, 50, 50), "#64#")) {
        _showRays = !_showRays;
    }
}

void World::handleMouseClicks() {
    switch (_userMode) {
        case OBSERVE: {
            break;
        }
        case DRAWING: {
            const bool isInBounds_x = 0 < GetMousePosition().x && GetMousePosition().x < GetScreenWidth() -
                                      _space_right;
            const bool isInBounds_y = 0 < GetMousePosition().y && GetMousePosition().y < GetScreenHeight() -
                                      _space_bottom;

            if (!isInBounds_x || !isInBounds_y) {
                _drawVar_borderStartPos.reset();
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (isInBounds_x && isInBounds_y) {
                    _drawVar_borderStartPos = GetMousePosition();
                }
            } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                if (_drawVar_borderStartPos.has_value()) {
                    _lines.addLine(_drawVar_borderStartPos.value(), GetMousePosition());
                }
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

void World::drawUserInfo() const {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 27);

    drawLineOfText(
        ("user mode: " + std::string(strFromUserMode())).c_str(),
        0
    );
    drawLineOfText(
        "----- Gen info -----",
        1
    );
    drawLineOfText(
        ("frames left: " + std::to_string(_generation_frameDuration - _frameCount)).c_str(),
        2
    );
    drawLineOfText(
        ("gen count: " + std::to_string(_generation_count)).c_str(),
        3
    );
    for (int i = 0; i < _populations.size(); i++) {
        drawLineOfText(
            ("Alive #" + std::to_string(i) + ": " + std::to_string(_populations[i].getAliveCount())).c_str(),
            4 + i
        );
    }
    int nextIdx = 4 + _populations.size();
    for (int i = 0; i < _populations.size(); i++) {
        drawLineOfText(
            std::string("Best #" + std::to_string(i) + ": " +
                        std::to_string(
                            static_cast<int>(
                                _populations[i]._best != nullptr
                                                 ? _populations[i]._best->calculateReward()
                                                 : 0
                            )
                        )
            ).c_str(),
            nextIdx + i
        );
    }
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


char *World::strFromUserMode() const {
    switch (_userMode) {
        case OBSERVE: return "observe";
        case DRAWING: return "draw";
        case MOVE_OBJECTS: return "move";
        default: return "?";
    }
}
