#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "raylib.h"
#include "raymath.h"
#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"
#include "worldengine/Population.h"
#include "worldengine/PopulationBuilder.h"
#include "worldengine/UI/TextureCollection.h"
#include "worldengine/World.h"
#include "worldengine/Lines.h"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000

#define DARKYELLOW Color(174, 184, 0, 255)

void draw();

void setGuiStyles();

// Initializing Obstacles
Lines lines = Lines()
        .addLine(Vector2{0, 0}, Vector2{SCREEN_WIDTH, 0})
        .addLine(Vector2{0, 0}, Vector2{0, SCREEN_HEIGHT})
        .addLine(Vector2{SCREEN_WIDTH, 0}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
        .addLine(Vector2{0, SCREEN_HEIGHT}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
        .addLine(Vector2{500, 0}, Vector2{500, 300})
        .addLine(Vector2{600, 450}, Vector2{600, 150});

// Layers
std::vector<Layer> layers = {
    Layer(30),
    Layer(8, Activation::tanh),
    Layer(4, Activation::tanh),
    Layer(2, Activation::tanh),
};

World *world = nullptr;

void gameLoop(std::stop_token stoken) {
    while (!stoken.stop_requested()) {
        world->act();
    }
}

int main() {
    // Setting random seed by the CPU time
    srand(time(nullptr));

    // Init the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    // raygui
    setGuiStyles();

    InitAudioDevice(); // Initialize audio device
    Music music = LoadMusicStream("assets/main_theme.wav");
    PlayMusicStream(music);

    SetTargetFPS(60);

    // loading all textures in the GPU at once
    TextureCollection::LoadAll();

    // World
    world = new World();

    // Populations
    std::unique_ptr<Population> yellowPopulation = PopulationBuilder(*world)
            .setCount(1000)
            .setElitePercentage(0.1)
            .setNetwork(layers, "")
            .setMutation(0.3, 0.2)
            .setPositions(Vector2{400, 300}, Vector2{50, 300})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 100, 60 * DEG2RAD) // 60°
            .setEntityTexture(TextureCollection::whiteAnt, DARKYELLOW)
            .build();
    std::unique_ptr<Population> redPopulation(
        PopulationBuilder(*world)
        .setCount(1000)
        .setElitePercentage(0.1)
        .setNetwork(layers, "")
        .setMutation(0.3, 0.2)
        .setPositions(Vector2{450, 700}, Vector2{50, 400})
        .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
        .setRays(30, 100, 60 * DEG2RAD) // 60°
        .setEntityTexture(TextureCollection::whiteAnt, RED)
        .build()
    );
    std::unique_ptr<Population> purplePopulation(PopulationBuilder(*world)
        .setCount(1000)
        .setElitePercentage(0.1)
        .setNetwork(layers, "")
        .setMutation(0.3, 0.2)
        .setPositions(Vector2{700, 500}, Vector2{50, 500})
        .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
        .setRays(30, 100, 60 * DEG2RAD) // 60°
        .setEntityTexture(TextureCollection::whiteAnt, DARKPURPLE)
        .build()
    );
    std::unique_ptr<Population> greenPopulation(
        PopulationBuilder(*world)
        .setCount(1000)
        .setElitePercentage(0.1)
        .setNetwork(layers, "")
        .setMutation(0.3, 0.2)
        .setPositions(Vector2{900, 200}, Vector2{50, 600})
        .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
        .setRays(30, 100, 60 * DEG2RAD) // 60°
        .setEntityTexture(TextureCollection::whiteAnt, DARKGREEN)
        .build()
    );
    std::vector<std::shared_ptr<Population> > populations{};

    populations.push_back(std::move(yellowPopulation));
    populations.push_back(std::move(redPopulation));
    populations.push_back(std::move(purplePopulation));
    populations.push_back(std::move(greenPopulation));

    world->setLines(lines)
            .setGenerationDuration(20 * 60)
            .setPopulations(std::move(populations));

    Lines::addRecord(30);
    for (float i = 50; i < 505; i += 5) {
        Lines::addRecord(i);
    }

    std::jthread t(gameLoop);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music); // Update music buffer with new stream data
        draw();
    }

    t.request_stop();
    UnloadMusicStream(music); // Unload music stream buffers from RAM
    CloseAudioDevice(); // Close audio device (music streaming is automatically stopped)
    TextureCollection::FreeAll();
    delete world;
    CloseWindow();
    return 0;
}

void draw() {
    BeginDrawing();
    ClearBackground(WHITE);

    world->draw();
    DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

    EndDrawing();
}

void setGuiStyles() {
}
