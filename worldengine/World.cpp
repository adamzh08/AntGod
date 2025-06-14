//
// Created by yanam on 09.06.2025.
//

#include "World.h"

#include <iostream>


World& World::setLines(const Lines &lines) {
    _lines = lines;
    return *this;
}
World& World::setPopulations(std::vector<Population>&& populations) {
    _populations = std::move(populations);
    return *this;
}
World& World::setGenerationDuration(int duration) {
    _generation_frameDuration = duration;
    return *this;
}

int frameCounter = 0;
void World::act() {
    frameCounter++;

    if (frameCounter == _generation_frameDuration) {
        frameCounter = 0;
        for (Population& population : _populations) {
            population.flood();
        }
    }
    for (Population& population : _populations) {
        population.act();
    }
}

void World::draw() const {
    // draw borders
    for (const Line& line : _lines._lines) {
        DrawLineEx(line.start, line.end, 1.5, BLACK);
    }

    // draw colonies
    for (const Population& population: _populations) {
        population.draw();
    }
    if (_rayStartPos.has_value()) {
        DrawLineV(GetMousePosition(), _rayStartPos.value(), BLACK);
    }

    const std::string framesLeft_str = std::to_string(_generation_frameDuration - frameCounter);

    DrawText((std::string("frames left ") + framesLeft_str).c_str(), GetScreenWidth() - 300, GetScreenHeight() - 25, 20, BLACK);
}

void World::handleUserInput() {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        std::cout << "pressed!" << std::endl;

        if (!_rayStartPos.has_value()) {
            _rayStartPos = GetMousePosition();
        } else {
            _rayEndPos = GetMousePosition();
            _lines._lines.push_back(Line(_rayStartPos.value(), _rayEndPos.value()));

            _rayStartPos.reset();
            _rayEndPos.reset();
        }
    }
}

