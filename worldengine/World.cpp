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

    const std::string framesLeft_str = std::to_string(_generation_frameDuration - frameCounter);

    DrawText((std::string("frames left ") + framesLeft_str).c_str(), GetScreenWidth() - 300, GetScreenHeight() - 25, 20, BLACK);
}
