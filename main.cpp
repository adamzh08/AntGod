#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "raygui.h"
#include "raylib.h"
#include "raymath.h"
#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"
#include "worldengine/Line.h"
#include "worldengine/Lines.h"
#include "worldengine/Population.h"
#include "worldengine/PopulationBuilder.h"
#include "worldengine/TextureCollection.h"
#include "worldengine/World.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720


void draw();

// Initializing Obstacles
const std::vector<Line> lines_conf = {
    Line(Vector2{500, 0}, Vector2{500, 300}),
    Line(Vector2{600, 450}, Vector2{600, 150}),
    Line(Vector2{700, 0}, Vector2{700, 300}),
};
const Lines lines{lines_conf};

// Layers
std::vector<Layer> layers = {
    Layer(2),
    Layer(8, Activation::tanh),
    Layer(2, Activation::tanh),
};

World *world = nullptr;

int main() {
    // Setting random seed by the CPU time
    srand(time(nullptr));


    // Init the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    // loading all textures in the GPU at once
    TextureCollection::LoadAll();

    // World
    world = new World();

    // Populations
    Population beesPopulation = PopulationBuilder(*world)
            .setCount(10000)
            .setElitePercentage(0.3)
            .setNetwork(layers, "")
            .setMutationProbability(0.3)
            .setPositions(Vector2{600, 300}, Vector2{50, 400})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 100)
            .setEntityTexture(TextureCollection::bee)
            .build();
    Population antsPopulation = PopulationBuilder(*world)
            .setCount(10000)
            .setElitePercentage(0.3)
            .setNetwork(layers, "")
            .setMutationProbability(0.3)
            .setPositions(Vector2{600, 500}, Vector2{50, 400})
            .setMovement(RADIAL_MOVE, 2, 10 * DEG2RAD)
            .setRays(30, 100)
            .setEntityTexture(TextureCollection::ant)
            .build();

    world->setLines(lines)
            .setLayout(300, 100)
            .setGenerationDuration(10 * 60)
            .setPopulations({
                beesPopulation,
                antsPopulation,
            });

    while (!WindowShouldClose()) {
        world->handleUserInput();
        world->act();
        draw();
    }

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
