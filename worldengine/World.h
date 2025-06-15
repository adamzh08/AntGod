//
// Created by yanam on 09.06.2025.
//

#ifndef WORLD_H
#define WORLD_H
#include <optional>

#include "Lines.h"
#include "Population.h"
#include "World.h"

enum UserMode {
    OBSERVE,
    DRAWING,
    MOVE_OBJECTS
};


class World {
private:
    int _generation_frameDuration{};
    int _generation_count{};
    int _frameCount{};
    bool _paused = false;

    // drawing
    UserMode _userMode = OBSERVE;
    std::optional<Vector2> _drawVar_borderStartPos;

    void drawGame() const;

    void drawUserInfo() const;

    void handleUserInput();
    void handleMouseClicks();
    void handleButtons();

    void drawLineOfText(const char *str, int idx) const;

    char *strFromUserMode() const;

public:
    Lines _lines{};
    std::vector<Population> _populations;

    int _space_right{};
    int _space_bottom{};

    World &setLines(const Lines &lines);

    World &setPopulations(std::vector<Population> &&populations);

    World &setGenerationDuration(int duration);

    World& setLayout(int space_right, int space_bottom);

    void act();

    void draw();
};


#endif //WORLD_H
