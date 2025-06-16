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
    EDIT_MAP
};
enum DrawAction {
    NONE,
    DRAW_WALL,
    DELETE_WALL,
    MOVE_COLONY_INIT,
    MOVE_COLONY_TARGET
};

class World {
private:
    bool _paused = false;

    // drawing
    UserMode _userMode = OBSERVE;
    bool _drawVar_hasRightClicked = false;
    Vector2 _drawVar_menuPos{};
    int _drawVar_action = NONE;
    int _drawVar_menuOptionsCount = 5;


    void drawGame();

    void drawUserInfo();

    void handleUserInput();
    void handleMouseClicks();
    void handleButtons();

    void drawLineOfText(const char *str, int idx) const;

    [[nodiscard]] char *strFromUserMode() const;
    [[nodiscard]] char *strFromDrawMode(int action) const;

public:
    Lines _lines;
    std::vector<Population> _populations;

    int _generation_frameDuration{};
    int _generation_count{};
    int _frameCount{};

    bool _showRays = false;

    World &setLines(const Lines &lines);

    World &setPopulations(std::vector<Population> &&populations);

    World &setGenerationDuration(int duration);

    void act();

    void draw();
};


#endif //WORLD_H
