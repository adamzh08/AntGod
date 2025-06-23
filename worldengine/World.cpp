//
// Created by yanam on 09.06.2025.
//

#include <iostream>
#include <string>

#include "World.h"

#define RAYGUI_IMPLEMENTATION

#include <algorithm>
#include <thread>

#include "LineIntersection.h"
#include "PopulationBuilder.h"
#include "UI/TextureCollection.h"
#include "../raygui.h"
#include "UI/ColorEditBox.h"
#include "UI/EvolutionEditBox.h"
#include "UI/RaysEditBox.h"


World &World::setLines(const Lines &lines) {
    _lines = lines;
    return *this;
}

World &World::setPopulations(std::vector<std::shared_ptr<Population> > populations) {
    _populations = populations;
    for (auto &pop: _populations) {
        pop->initAnts();
    }
    reconstructInfoBoxes();
    return *this;
}

World &World::setGenerationDuration(int duration) {
    _generation_frameDuration = duration;
    return *this;
}

void World::act() {
    if (_UI_needsPops.load()) {
        std::this_thread::yield();
        return;
    }
    if (!_paused) {
        std::unique_lock<std::mutex> lock(_populationMutex, std::try_to_lock);
        if (!lock.owns_lock()) {
            std::this_thread::yield();
            return;
        }

        _frameCount++;

        if (_frameCount == _generation_frameDuration) {
            for (auto &pop: _populations) {
                pop->flood();
            }
            _generation_count++;
            _frameCount = 0;
        }
        for (auto &pop: _populations) {
            if (!pop) {
                std::cerr << "Null population" << std::endl;
                continue;
            }
            pop->act();
        }
    }
}

void World::draw() {
    _frameCounter_graphUpdate++;

    drawGame();
    handleUserInput();

    if (_showInfo) {
        drawUserInfo();
        displayInfoBoxes();
    }

    // graphs
    if (_frameCounter_graphUpdate == _framesPerGraphUpdate) {
        _frameCounter_graphUpdate = 0;
        updateInfoBoxes();
    }
}

void World::drawGame() {
    // obstacles
    _lines.draw();

    // colonies
    for (auto &population: _populations) {
        population->draw();
    }

    switch (_drawVar_action) {
        case NONE:
        case DELETE_WALL:
        case DELETE_COLONY: break;

        case DRAW_WALL: {
            DrawLineV(_drawVar_menuPos, GetMousePosition(), BLUE);
            break;
        }
        case MOVE_COLONY_INIT: {
            Population::drawXAt(GetMousePosition(), _populations[findClickedColonyIndex().value()]->_entityColor);
            break;
        }
        case MOVE_COLONY_TARGET: {
            Population::drawFlagAt(GetMousePosition(), _populations[findClickedTargetIndex().value()]->_entityColor);
            break;
        }
        case CREATE_COLONY: {
            Population::drawXAt(_drawVar_menuPos, WHITE);
            Population::drawFlagAt(GetMousePosition(), WHITE);
            break;
        }
        default: std::cerr << "invalid action" << std::endl;
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
    const Color buttonBlue = Fade((Color){100, 100, 255}, 0.5f);
    const Color buttonGreen = Fade((Color){0, 255, 0}, 0.5f);

    // GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, ColorToInt(buttonBlue));
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(Fade(WHITE, 0.8f)));

    auto toggleButton = [buttonBlue, buttonGreen](const Rectangle rect, const char *label, const bool active) {
        GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(active ? buttonGreen : buttonBlue));
        return GuiButton(rect, label);
    };

    // switch to normal mode button
    if (toggleButton(Rectangle(0, GetScreenHeight() - 60, 100, 50), "Observe", _userMode == OBSERVE)) {
        _drawVar_action = NONE;
        _drawVar_hasRightClicked = false;
        _userMode = OBSERVE;
    }
    // switch to draw mode button
    if (toggleButton(Rectangle(125, GetScreenHeight() - 60, 100, 50), "Edit", _userMode == EDIT_MAP)) {
        _userMode = EDIT_MAP;
    }

    // info for RayGui icons: in raygui.h, look at enum 'GuiIconName' with ctrl + f to find the icon ids fast
    // pause button
    if (toggleButton(Rectangle(375, GetScreenHeight() - 60, 50, 50), _paused ? "#131#" : "#132#", _paused)) {
        _paused = !_paused;
    }
    if (toggleButton(Rectangle(450, GetScreenHeight() - 60, 50, 50), "#64#", _showRays)) {
        _showRays = !_showRays;
    }

    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt(buttonBlue));
    if (GuiButton(Rectangle(GetScreenWidth() - (_showInfo ? 450 : 50), 0, 50, 50), _showInfo ? "#115#" : "#114#")) {
        _showInfo = !_showInfo;
    }
    if (_showInfo) {
        if (GuiButton(Rectangle(GetScreenWidth() - 400, GetScreenHeight() / 2 - 60, 50, 50), "#114#")) {
            _shownBoxTypeIdx--;
            _shownBoxTypeIdx = (_shownBoxTypeIdx + _allInfoBoxes.size()) % _allInfoBoxes.size();
        }
        if (GuiButton(Rectangle(GetScreenWidth() - 50, GetScreenHeight() / 2 - 60, 50, 50), "#115#")) {
            _shownBoxTypeIdx++;
            _shownBoxTypeIdx %= _allInfoBoxes.size();
        }
    }

    // the edit menu
    if (_drawVar_hasRightClicked) {
        const Color niceButtonGreen = Fade((Color){0, 0, 0}, 0.75f);

        Rectangle menuRect{
            _drawVar_menuPos.x,
            _drawVar_menuPos.y,
            200,
            1.2f * _drawVar_editMenuButtonHeight * _drawVar_menuOptionsCount
        };
        menuRect.x = std::min(menuRect.x, GetScreenWidth() - menuRect.width);
        menuRect.y = std::min(menuRect.y, GetScreenHeight() - menuRect.height);
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
                              menuRect.x,
                              menuRect.y + menuRect.height * i / _drawVar_menuOptionsCount,
                              200,
                              _drawVar_editMenuButtonHeight
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
        _drawVar_action = NONE;
    }
    switch (_drawVar_action) {
        case NONE:
        case DELETE_WALL:
        case DELETE_COLONY: break;

        case DRAW_WALL: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // adding a new line
                _lines.addLine(_drawVar_menuPos, GetMousePosition());
                _drawVar_action = NONE;
            }
            break;
        }
        case MOVE_COLONY_INIT: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // moving the colony init position
                _populations[findClickedColonyIndex().value()]->_init_position = GetMousePosition();
                _drawVar_action = NONE;
            }
            break;
        }
        case MOVE_COLONY_TARGET: {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                // moving the colony target position
                _populations[findClickedTargetIndex().value()]->_target_position = GetMousePosition();
                _drawVar_action = NONE;
            }
            break;
        }
        case CREATE_COLONY: {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                auto randC = []() {
                    return static_cast<unsigned char>(static_cast<float>(rand()) / RAND_MAX * 255);
                };
                const Color randomColor{randC(), randC(), randC(), 255};

                // default config
                auto newPop = PopulationBuilder(*this)
                        .setCount(100)
                        .setElitePercentage(0.3)
                        .setNetwork(_populations[0]->_layers, "")
                        .setMutation(0.1, 0.2)
                        .setPositions(_drawVar_menuPos, GetMousePosition())
                        .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
                        .setRays(30, 100, 60 * DEG2RAD) // 60°
                        .setEntityTexture(TextureCollection::whiteAnt, randomColor)
                        .build();

                _UI_needsPops.store(true);
                std::lock_guard<std::mutex> lock(_populationMutex);
                _populations.push_back(std::move(newPop));
                _populations.back()->initAnts();
                _UI_needsPops.store(false);

                reconstructInfoBoxes();
                _drawVar_action = NONE;
            }
            break;
        }
        default: std::cerr << "invalid action" << std::endl;
    }
}

void World::afterEditOptionSelected() {
    switch (_drawVar_action) {
        case NONE:
        case DRAW_WALL:
        case MOVE_COLONY_INIT:
        case MOVE_COLONY_TARGET:
        case CREATE_COLONY: return;
        case DELETE_WALL: {
            const int lineIdx = findIntersectingWallRayIndex(_drawVar_menuPos, _pickRadius, 16).value();
            _lines._lines.erase(_lines._lines.begin() + lineIdx);
            break;
        }
        case DELETE_COLONY: {
            const int popIdx = findClickedColonyIndex().value();

            _UI_needsPops.store(true);
            std::lock_guard<std::mutex> lock(_populationMutex);
            _populations.erase(_populations.begin() + popIdx);
            _UI_needsPops.store(false);

            reconstructInfoBoxes();
            break;
        }
        default: std::cerr << "invalid action" << std::endl;
    }
}


bool World::menuOptionAvailable(const int option) const {
    switch (option) {
        case NONE:
        case DRAW_WALL:
        case CREATE_COLONY: return true; // always available
        case DELETE_WALL: {
            return findIntersectingWallRayIndex(_drawVar_menuPos, _pickRadius, 16).has_value();
        }
        case MOVE_COLONY_INIT: {
            return findClickedColonyIndex().has_value();
        }
        case MOVE_COLONY_TARGET: {
            return findClickedTargetIndex().has_value();
        }
        case DELETE_COLONY: {
            return findClickedColonyIndex().has_value();
        }
        default: return false;
    }
}


void World::drawUserInfo() const {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 27);

    drawLineOfText(
        "----- Generation -----",
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

    drawLineOfText(
        "----- General -----",
        3
    );
    drawLineOfText(
        "Edit mode + right click = menu",
        4
    );

    GuiDrawText(
        _infoBoxDescriptions[_shownBoxTypeIdx],
        Rectangle(GetScreenWidth() - 350, GetScreenHeight() / 2 - 50, 300, 50),
        TEXT_ALIGN_CENTER,
        BLACK
    );

    float soundValue = GetMasterVolume();
    GuiSlider(
        Rectangle{
            GetScreenWidth() - 325.f,
            GetScreenHeight() / 2.f - 150,
            250.f,
            50.f
        }, "leise", "laut",
        &soundValue,
        0,
        1
    );
    SetMasterVolume(soundValue);
}

void World::updateInfoBoxes() {
    for (int i = 0; i < _populations.size(); i++) {
        // graphs
        dynamic_cast<Graph *>(_aliveGraphs[i])->addPoint(
            _generation_frameDuration * _generation_count + _frameCount,
            _populations[i]->getAliveCount()
        );
        dynamic_cast<Graph *>(_bestDistGraphs[i])->addPoint(
            _generation_frameDuration * _generation_count + _frameCount,
            _populations[i]->getBestDist()
        );
        dynamic_cast<Graph *>(_avgDistGraphs[i])->addPoint(
            _generation_frameDuration * _generation_count + _frameCount,
            _populations[i]->getAvgDist()
        );
    }
}

void World::displayInfoBoxes() const {
    for (int i = 0; i < _populations.size(); i++) {
        _allInfoBoxes[_shownBoxTypeIdx]->at(i)->drawBounds();
        _allInfoBoxes[_shownBoxTypeIdx]->at(i)->draw();
    }
}

void World::reconstructInfoBoxes() {
    _allInfoBoxes.clear();

    _aliveGraphs.clear();
    _bestDistGraphs.clear();
    _avgDistGraphs.clear();
    _sensorBoxes.clear();
    _evolutionBoxes.clear();
    _colorBoxes.clear();
    _neuroBoxes.clear();

    for (int i = 0; i < _populations.size(); i++) {
        const Rectangle boxPos(
            GetScreenWidth() - 400,
            GetScreenHeight() / 2 * (1 + static_cast<float>(i) / _populations.size()),
            400,
            GetScreenHeight() / 2 / _populations.size() - 15
        );
        // graphs
        _aliveGraphs.push_back(new Graph(boxPos, _populations[i]));
        _bestDistGraphs.push_back(new Graph(boxPos, _populations[i]));
        _avgDistGraphs.push_back(new Graph(boxPos, _populations[i]));
        // parameters
        _sensorBoxes.push_back(new RaysEditBox(_populations[i], boxPos));
        _evolutionBoxes.push_back(new EvolutionEditBox(_populations[i], boxPos));
        _colorBoxes.push_back(new ColorEditBox(_populations[i], boxPos));
        // brains
        _neuroBoxes.push_back(new NeuroBox(boxPos, _populations[i]));
    }

    _allInfoBoxes.push_back(&_aliveGraphs);
    _allInfoBoxes.push_back(&_bestDistGraphs);
    _allInfoBoxes.push_back(&_avgDistGraphs);
    _allInfoBoxes.push_back(&_sensorBoxes);
    _allInfoBoxes.push_back(&_evolutionBoxes);
    _allInfoBoxes.push_back(&_colorBoxes);
    _allInfoBoxes.push_back(&_neuroBoxes);
}

std::optional<int> World::findIntersectingWallRayIndex(Vector2 origin, float radius, int rayCount) const {
    Lines::addRecord(radius);
    const std::vector<Vector2> deltas = _lines._getRaysPoints(radius, rayCount, 0, 2 * PI);
    const std::vector<float> dists = _lines.getRays(origin, radius, rayCount, 0, 2 * PI);

    int bestIdx = -1;
    float bestValue = -1;

    for (int i = 0; i < rayCount; i++) {
        if (dists[i] > bestValue) {
            bestValue = dists[i];
            bestIdx = i;
        }
    }

    if (bestValue > 0 && bestIdx >= 0) {
        const Vector2 rayEnd = Vector2{
            origin.x + deltas[bestIdx].x,
            origin.y + deltas[bestIdx].y
        };

        for (int j = 0; j < _lines._lines.size(); j++) {
            const Line &line = _lines._lines[j];

            if (doIntersect(origin, rayEnd, line.startPoint, line.endPoint)) {
                return j;
            }
        }
        std::cerr << "Warning: best ray had value > 0, but no intersection was found." << std::endl;
    }

    return std::nullopt;
}

std::optional<int> World::findClickedColonyIndex() const {
    for (int i = 0; i < _populations.size(); i++) {
        const bool clicked_x = _populations[i]->_init_position.x - _pickRadius < _drawVar_menuPos.x &&
                               _drawVar_menuPos.x < _populations[i]->_init_position.x + _pickRadius;
        const bool clicked_y = _populations[i]->_init_position.y - _pickRadius < _drawVar_menuPos.y &&
                               _drawVar_menuPos.y < _populations[i]->_init_position.y + _pickRadius;
        if (clicked_x && clicked_y) {
            return i;
        }
    }
    return std::nullopt;
}

std::optional<int> World::findClickedTargetIndex() const {
    for (int i = 0; i < _populations.size(); i++) {
        const bool clicked_x = _populations[i]->_target_position.x - _pickRadius < _drawVar_menuPos.x &&
                               _drawVar_menuPos.x < _populations[i]->_target_position.x + _pickRadius;
        const bool clicked_y = _populations[i]->_target_position.y - _pickRadius < _drawVar_menuPos.y &&
                               _drawVar_menuPos.y < _populations[i]->_target_position.y + _pickRadius;
        if (clicked_x && clicked_y) {
            return i;
        }
    }
    return std::nullopt;
}

void World::drawLineOfText(const char *str, const int idx) {
    GuiDrawText(
        str,
        Rectangle(
            GetScreenWidth() - 400,
            10 + idx * GuiGetStyle(DEFAULT, TEXT_SIZE),
            400,
            10
        ),
        TEXT_ALIGN_CENTER,
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

const char *World::strFromDrawMode(const int action) {
    switch (action) {
        case NONE: return "Close";
        case DRAW_WALL: return "Draw Wall";
        case DELETE_WALL: return "Delete Wall";
        case MOVE_COLONY_INIT: return "Move Origin";
        case MOVE_COLONY_TARGET: return "Move Target";
        case CREATE_COLONY: return "Create Colony";
        case DELETE_COLONY: return "Delete Colony";
        default: std::cerr << "undefined action" << std::endl;
    }
    return "?";
}
