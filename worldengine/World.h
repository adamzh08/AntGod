//
// Created by yanam on 09.06.2025.
//

#ifndef WORLD_H
#define WORLD_H
#include <optional>

#include "Graph.h"
#include "Lines.h"
#include "Population.h"
#include "World.h"

enum UserMode {
    OBSERVE,
    EDIT_MAP
};

enum DrawAction {
    NONE,
    DRAW_WALL, // Done
    DELETE_WALL, // Todo
    MOVE_COLONY_INIT, // Done
    MOVE_COLONY_TARGET // Done
};

class World {
private:
    // graphs
    std::vector<std::vector<Graph> *> _allGraphs;
    std::vector<Graph> _aliveGraphs{};
    std::vector<Graph> _bestRewardGraphs{};
    std::vector<Graph> _avgDistGraphs{};
    std::vector<const char *> _graphDescriptions{
        "How many entities alive?",
        "Best dist to goal?",
        "Average dist to goal?"
    };
    int _framesPerGraphUpdate = 3;
    int _frameCounter_graphUpdate{};

    int _shownGraphTypeIdx{};

    bool _showInfo = true;
    bool _paused = false;

    // drawing
    UserMode _userMode = OBSERVE;
    bool _drawVar_hasRightClicked = false;
    Vector2 _drawVar_menuPos{};
    int _drawVar_action = NONE;
    int _drawVar_menuOptionsCount = 5;
    float _pickRadius = 20;

    // colony movement
    int _drawVar_popIdxClicked = -1;


    void drawGame();

    void drawUserInfo() const;

    void updateGraphs();

    void displayGraphs() const;

    void handleUserInput();

    void handleMapEditing();

    void handleButtons();

    void afterEditOptionSelected();

    [[nodiscard]] bool menuOptionAvailable(int option) const;

    static void drawLineOfText(const char *str, int idx);

    [[nodiscard]] const char *strFromUserMode() const;

    [[nodiscard]] const char *strFromDrawMode(int action) const;

public:
    Lines _lines;
    std::vector<Population> _populations;

    int _generation_frameDuration{};
    int _generation_count{};
    int _frameCount{};

    bool _showRays = false;

    World() = default;

    World &setLines(const Lines &lines);

    World &setPopulations(std::vector<Population> &&populations);

    World &setGenerationDuration(int duration);

    void act();

    void draw();
};


#endif //WORLD_H
