//
// Created by yanam on 09.06.2025.
//

#include "World.h"

#include <iostream>


World& World::setLines(Lines lines) {
    _lines = lines;
    return *this;
}

World& World::setPopulations(std::vector<Population> populations) {
    _populations = populations;
    return *this;
}

int frameCounter = 0;
void World::act() {
    frameCounter++;

    if (frameCounter == 2 * 60) {
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
}
