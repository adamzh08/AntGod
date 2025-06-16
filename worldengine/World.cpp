//
// Created by yanam on 09.06.2025.
//

#include <iostream>
#include <string>

#include "World.h"

#define RAYGUI_IMPLEMENTATION

#include <algorithm>

#include "../raygui.h"

World::World() {
}

World &World::setLines(const Lines &lines) {
    _lines = lines;
    return *this;
}

World &World::setPopulations(std::vector<Population> &&populations) {
    _populations = std::move(populations);

    // graphs
    for (int i = 0; i < _populations.size(); i++) {
        _graphs.emplace_back(Rectangle(
            GetScreenWidth() - 400,
            GetScreenHeight() / 2 * (1 + static_cast<float>(i) / _populations.size()),
            400,
            GetScreenHeight() / 2 / _populations.size() - 15
        ));
    }

    return *this;
}

World &World::setGenerationDuration(int duration) {
    _generation_frameDuration = duration;
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
    if (_showInfo) {
        drawUserInfo();
    }
}

void World::drawGame() {
    // obstacles
    _lines.draw();

    // colonies
    for (Population &population: _populations) {
        population.draw();
    }

    switch (_drawVar_action) {
        case NONE: break; // nothing is edited currently
        case DRAW_WALL: {
            DrawLineV(_drawVar_menuPos, GetMousePosition(), BLUE);
            break;
        }
        case DELETE_WALL: {
            // Todo implement
            break;
        }
        case MOVE_COLONY_INIT:
        case MOVE_COLONY_TARGET: {
            DrawCircleV(GetMousePosition(), 10, ORANGE);
            break;
        }
    }
}

void World::handleUserInput() {
    if (_userMode == EDIT_MAP) {
        handleMapEditing();
    }
    handleButtons();
}


void World::handleButtons() {
    // text size
    GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
    // button
    const Color niceButtonBlue = Fade((Color){100, 100, 255}, 0.5f);
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(niceButtonBlue));
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(niceButtonBlue));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(Fade(WHITE, 0.8f)));


    // switch to normal mode button
    if (GuiButton(Rectangle(0, GetScreenHeight() - 60, 100, 50), "Observe")) {
        _drawVar_action = NONE;
        _drawVar_hasRightClicked = false;
        _userMode = OBSERVE;
    }
    // switch to draw mode button
    if (GuiButton(Rectangle(125, GetScreenHeight() - 60, 100, 50), "Edit")) {
        _userMode = EDIT_MAP;
    }

    // info for RayGui icons: in raygui.h, look at enum 'GuiIconName' with ctrl + f to find the icon ids fast
    // pause button
    if (GuiButton(Rectangle(375, GetScreenHeight() - 60, 50, 50), _paused ? "#131#" : "#132#")) {
        _paused = !_paused;
    }
    if (GuiButton(Rectangle(450, GetScreenHeight() - 60, 50, 50), "#64#")) {
        _showRays = !_showRays;
    }
    if (GuiButton(Rectangle(GetScreenWidth() - (_showInfo ? 450 : 50), 0, 50, 50), _showInfo ? "#115#" : "#114#")) {
        _showInfo = !_showInfo;
    }

    // the edit menu
    if (_drawVar_hasRightClicked) {
        const Color niceButtonGreen = Fade((Color){0, 0, 0}, 0.75f);

        Rectangle menuRect{
            _drawVar_menuPos.x,
            _drawVar_menuPos.y,
            200,
            200
        };
        for (int i = 0; i < _drawVar_menuOptionsCount; i++) {
            bool available = menuOptionAvailable(i);
            if (available) {
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(niceButtonGreen));
                GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(niceButtonGreen));
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(Fade(WHITE, 0.8f)));
            } else {
                GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(niceButtonGreen));
                GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(niceButtonGreen));
                GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(Fade(WHITE, 0.8f)));
            }
            if (!available) GuiDisable();
            if (GuiButton({
                              _drawVar_menuPos.x,
                              _drawVar_menuPos.y + 200 * i / _drawVar_menuOptionsCount,
                              200,
                              30
                          }, strFromDrawMode(i))) {
                _drawVar_hasRightClicked = false;
                _drawVar_action = i;
                afterEditOptionSelected();
            }
            if (!available) GuiEnable();
        }

        DrawRectangleRec(menuRect, Fade(DARKGRAY, 0.5f));
        DrawRectangleLinesEx(menuRect, 1, GRAY);
    }
}

void World::handleMapEditing() {
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        // trigger menu popup
        _drawVar_hasRightClicked = true;
        _drawVar_menuPos = GetMousePosition();
    }
    switch (_drawVar_action) {
        case NONE: break;
        case DRAW_WALL: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // adding a new line
                _lines.addLine(_drawVar_menuPos, GetMousePosition());
                _drawVar_action = NONE;
            }
            break;
        }
        case DELETE_WALL: {
            // Todo implement
            break;
        }
        case MOVE_COLONY_INIT: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // moving the colony init position
                _populations[_drawVar_popIdxClicked]._init_position = GetMousePosition();
                _drawVar_action = NONE;
            }
            break;
        }
        case MOVE_COLONY_TARGET: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // moving the colony target position
                _populations[_drawVar_popIdxClicked]._target_position = GetMousePosition();
                _drawVar_action = NONE;
            }
            break;
        }
    }
}

void World::afterEditOptionSelected() {
    switch (_drawVar_action) {
        case NONE:
        case DRAW_WALL: return;
        case DELETE_WALL: {
            // Todo
            return;
        }
        case MOVE_COLONY_INIT: {
            for (int i = 0; i < _populations.size(); i++) {
                const bool clicked_x = _populations[i]._init_position.x - 10 < _drawVar_menuPos.x &&
                                       _drawVar_menuPos.x < _populations[i]._init_position.x + 10;
                const bool clicked_y = _populations[i]._init_position.y - 10 < _drawVar_menuPos.y &&
                                       _drawVar_menuPos.y < _populations[i]._init_position.y + 10;
                if (clicked_x && clicked_y) {
                    _drawVar_popIdxClicked = i;
                    return;
                }
            }
        }
        case MOVE_COLONY_TARGET: {
            for (int i = 0; i < _populations.size(); i++) {
                const bool clicked_x = _populations[i]._target_position.x - 10 < _drawVar_menuPos.x &&
                                       _drawVar_menuPos.x < _populations[i]._target_position.x + 10;
                const bool clicked_y = _populations[i]._target_position.y - 10 < _drawVar_menuPos.y &&
                                       _drawVar_menuPos.y < _populations[i]._target_position.y + 10;
                if (clicked_x && clicked_y) {
                    _drawVar_popIdxClicked = i;
                    return;
                }
            }
        }
    }
}


bool World::menuOptionAvailable(const int option) const {
    switch (option) {
        case NONE:
        case DRAW_WALL: return true; // always available
        case DELETE_WALL: return false; // Todo
        case MOVE_COLONY_INIT: {
            for (const Population &_population: _populations) {
                const bool clicked_x = _population._init_position.x - 10 < _drawVar_menuPos.x &&
                                       _drawVar_menuPos.x < _population._init_position.x + 10;
                const bool clicked_y = _population._init_position.y - 10 < _drawVar_menuPos.y &&
                                       _drawVar_menuPos.y < _population._init_position.y + 10;
                if (clicked_x && clicked_y) {
                    return true;
                }
            }
            return false;
        }
        case MOVE_COLONY_TARGET: {
            for (const Population &_population: _populations) {
                const bool clicked_x = _population._target_position.x - 10 < _drawVar_menuPos.x &&
                                       _drawVar_menuPos.x < _population._target_position.x + 10;
                const bool clicked_y = _population._target_position.y - 10 < _drawVar_menuPos.y &&
                                       _drawVar_menuPos.y < _population._target_position.y + 10;
                if (clicked_x && clicked_y) {
                    return true;
                }
            }
            return false;
        }
        default: return false;
    }
}


void World::drawUserInfo() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 27);

    drawLineOfText(
        ("user mode: " + std::string(strFromUserMode())).c_str(),
        0
    );
    drawLineOfText(
        "----- Generation -----",
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
        /*
        if (!_paused) {
            _populations[i].getAntsHistory();
        }
        for (int j = 0; j < GetScreenWidth(); j++) {
            DrawPixel(
                j, _populations[i]._ants_amount * 0.2 - (
                       _populations[i]._sizeHistory[_populations[i]._sizeHistory.size() * j / GetScreenWidth()] * 0.2),
                BLACK);

        drawLineOfText(
            ("Alive #" + std::to_string(i) + ": " + std::to_string(_populations[i].getAliveCount())).c_str(),
            4 + i
        );
        }*/
        _graphs[i].addPoint(
            _generation_frameDuration * _generation_count + _frameCount,
            _populations[i].getAliveCount()
        );
        _graphs[i].draw();
    }

    int nextIdx = 4; // + _populations.size();
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
    nextIdx += _populations.size();

    drawLineOfText(
        "----- General -----",
        nextIdx
    );
    drawLineOfText(
        "Green dots: initial positions",
        nextIdx + 1
    );
    drawLineOfText(
        "Red dots: target positions",
        nextIdx + 2
    );
    drawLineOfText(
        "Edit mode + right click = menu",
        nextIdx + 3
    );
}


void World::drawLineOfText(const char *line, int idx) {
    GuiDrawText(
        line,
        Rectangle(
            GetScreenWidth() - 400,
            10 + idx * GuiGetStyle(DEFAULT, TEXT_SIZE),
            400,
            10
        ),
        TEXT_ALIGN_LEFT | TEXT_ALIGN_TOP,
        BLACK
    );
}


const char *World::strFromUserMode() const {
    switch (_userMode) {
        case OBSERVE: return "observe";
        case EDIT_MAP: return "edit";
        default: return "?";
    }
}

const char *World::strFromDrawMode(int action) const {
    switch (action) {
        case NONE: return "None";
        case DRAW_WALL: return "Draw Wall";
        case DELETE_WALL: return "Delete Wall";
        case MOVE_COLONY_INIT: return "Move Colony Init";
        case MOVE_COLONY_TARGET: return "Move Colony Target";
        default: std::cerr << "undefined action" << std::endl;
    }
}
