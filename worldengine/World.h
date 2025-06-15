//
// Created by yanam on 09.06.2025.
//

#ifndef WORLD_H
#define WORLD_H
#include "Lines.h"
#include "Population.h"
#include "World.h"

enum UserMode {
    JUST_LOOKING,
    DRAWING,
    MOVE_OBJECTS
};


class World {
private:
    int frameCounter = 0;

    // drawing
    UserMode _userMode = JUST_LOOKING;
    std::optional<Vector2> _drawVar_borderStartPos;

    void drawGame() const;
    void drawUserInfo() const;


    void drawButtons();
    void drawLineOfText(const char* str, int idx) const;

    char* strFromUserMode() const;
public:
    Lines _lines{};
    std::vector<Population> _populations;
    int _generation_frameDuration;

    World& setLines(const Lines &lines);
    World& setPopulations(std::vector<Population>&& populations);
    World& setGenerationDuration(int duration);

    void act();
    void draw();
    void handleUserInput();
};



#endif //WORLD_H
