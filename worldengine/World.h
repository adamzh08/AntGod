//
// Created by yanam on 09.06.2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <atomic>

#include "UI/Graph.h"
#include "Lines.h"
#include "UI/NeuroBox.h"
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
    // infoboxes
    std::vector<std::vector<InfoBox *> *> _allInfoBoxes{};
    std::vector<InfoBox *> _aliveGraphs{};
    std::vector<InfoBox *> _bestDistGraphs{};
    std::vector<InfoBox *> _avgDistGraphs{};
    std::vector<InfoBox *> _sensorBoxes{};
    std::vector<InfoBox *> _evolutionBoxes{};
    std::vector<InfoBox *> _neuroBoxes{};
    std::vector<const char *> _infoBoxDescriptions{
        "How many entities alive?",
        "Best dist to goal?",
        "Average dist to goal?",
        "Environment Sensors?",
        "Evolutionary Params?",
        "Brain of the best?"
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
    float _pickRadius = 40;

    // colony movement
    int _drawVar_popIdxClicked = -1;


    void drawGame();

    void drawUserInfo() const;

    void updateInfoBoxes();

    void displayInfoBoxes() const;

    void handleUserInput();

    void handleMapEditing();

    void handleButtons();

    void afterEditOptionSelected();

    [[nodiscard]] bool menuOptionAvailable(int option) const;

    static void drawLineOfText(const char *str, int idx);

    [[nodiscard]] const char *strFromUserMode() const;

    [[nodiscard]] static const char *strFromDrawMode(int action) ;

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
