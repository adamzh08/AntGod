#include <cassert>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"
#include "worldengine/Population.h"
#include "worldengine/PopulationBuilder.h"
#include "worldengine/TextureCollection.h"
#include "worldengine/World.h"
#include "worldengine/Lines.h"

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 1000

void draw();
void setGuiStyles();

// Initializing Obstacles
Lines lines = Lines()
            .addLine(Vector2{0, 0}, Vector2{SCREEN_WIDTH, 0})
            .addLine(Vector2{0, 0}, Vector2{0, SCREEN_HEIGHT})
            .addLine(Vector2{SCREEN_WIDTH, 0}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
            .addLine(Vector2{0, SCREEN_HEIGHT}, Vector2{SCREEN_WIDTH, SCREEN_HEIGHT})
            .addLine(Vector2{500, 0}, Vector2{500, 300})
            .addLine(Vector2{600, 450}, Vector2{600, 150})
            .addLine(Vector2{600, 450}, Vector2{600, 150});

// Layers
std::vector<Layer> layers = {
    Layer(30),
    Layer(16, Activation::tanh),
    Layer(16, Activation::tanh),
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

    InitAudioDevice();              // Initialize audio device
    Music music = LoadMusicStream("assets/main_theme.wav");
    PlayMusicStream(music);

    SetTargetFPS(60);

    // loading all textures in the GPU at once
    TextureCollection::LoadAll();

    // World
    world = new World();

    // Populations
    Population beesPopulation = PopulationBuilder(*world)
            .setCount(1000)
            .setElitePercentage(0.3)
            .setNetwork(layers, "")
            .setMutationProbability(0.3)
            .setPositions(Vector2{650, 300}, Vector2{50, 400})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 100)
            .setEntityTexture(TextureCollection::bee)
            .build();
    Population antsPopulation = PopulationBuilder(*world)
            .setCount(1000)
            .setElitePercentage(0.3)
            .setNetwork(layers, "")
            .setMutationProbability(0.3)
            .setPositions(Vector2{600, 500}, Vector2{50, 400})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 200)
            .setEntityTexture(TextureCollection::ant)
            .build();

    world->setLines(lines)
            .setGenerationDuration(10 * 60)
            .setPopulations({
                beesPopulation,
                antsPopulation,
            });

    std::jthread t(gameLoop);

    while (!WindowShouldClose()) {
        UpdateMusicStream(music);   // Update music buffer with new stream data
        draw();
    }

    t.request_stop();
    UnloadMusicStream(music);   // Unload music stream buffers from RAM
    CloseAudioDevice();         // Close audio device (music streaming is automatically stopped)
    // free all
    TextureCollection::FreeAll();
    delete world;
    CloseWindow();
    return 0;
}

void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    world->draw();
    DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

    EndDrawing();
}

void setGuiStyles() {
}