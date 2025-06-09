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
    std::vector<Population> _populations{};

    World& setLines(Lines lines);
    World& setPopulations(std::vector<Population> populations);

    void act();
    void draw() const;
};



#endif //WORLD_H
