#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "raylib.h"
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


// Initializing Obstacles
Lines lines = Lines()
        .addLine(Vector2{0, 0}, Vector2{SCREEN_WIDTH, 0})
        .addLine(Vector2{0, 0}, Vector2{0, SCREEN_HEIGHT})
        .addLine(Vector2{SCREEN_WIDTH, 0}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
        .addLine(Vector2{0, SCREEN_HEIGHT}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
        .addLine(Vector2{500, 0}, Vector2{525, 325})
        .addLine(Vector2{600, 450}, Vector2{400, 350})
        .addLine(Vector2{800, 450}, Vector2{800, 850})
        .addLine(Vector2{200, 450}, Vector2{350, 750});

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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "prometheus simulator");


    InitAudioDevice(); // Initialize audio device
    Music music = LoadMusicStream("assets/main_theme.wav");
    PlayMusicStream(music);

    SetTargetFPS(60);

    // loading all textures in the GPU at once
    TextureCollection::LoadAll();

    // World
    world = new World();

    // Populations
    std::unique_ptr<Population> purplePopulation = PopulationBuilder(*world)
            .setCount(10000)
            .setElitePercentage(0.1)
            .setMutation(0.3, 0.2, 0.5, 0.1)
            .setPositions(Vector2{700, 500}, Vector2{50, 700})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 400, 60 * DEG2RAD) // 60°
            .setEntityTexture(TextureCollection::whiteAnt, DARKPURPLE)
            .build();
    std::unique_ptr<Population> greenPopulation = PopulationBuilder(*world)
            .setCount(10000)
            .setElitePercentage(0.1)
            .setMutation(0.3, 0.2, 0.5, 0.1)
            .setPositions(Vector2{800, 300}, Vector2{50, 150})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 400, 60 * DEG2RAD) // 60°
            .setEntityTexture(TextureCollection::whiteAnt, DARKGREEN)
            .build();


    std::vector<std::shared_ptr<Population> > populations{};
    populations.push_back(std::move(purplePopulation));
    populations.push_back(std::move(greenPopulation));

    world->setLines(lines)
            .setGenerationDuration(20 * 60)
            .setPopulations(std::move(populations));

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
