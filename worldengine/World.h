//
// Created by yanam on 09.06.2025.
//

#ifndef WORLD_H
#define WORLD_H
#include "Lines.h"
#include "Population.h"


class World {
public:
    Lines _lines{};
    std::vector<Population> _populations;
    int _generation_frameDuration;

    World& setLines(const Lines &lines);
    World& setPopulations(std::vector<Population>&& populations);
    World& setGenerationDuration(int duration);

    void act();
    void draw() const;
};



#endif //WORLD_H
