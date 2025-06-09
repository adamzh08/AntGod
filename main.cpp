#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"
#include "worldengine/Line.h"
#include "worldengine/Lines.h"
#include "worldengine/Population.h"
#include "worldengine/TextureCollection.h"

#define SCREEN_WIDTH 1080
#define SCREEN_HEIGHT 720

// Initializing Obstacles
const std::vector<Line> lines_conf = {
    Line(Vector2{500, 0}, Vector2{500, 300}),
    Line(Vector2{600, 450}, Vector2{600, 150}),
    Line(Vector2{700, 0}, Vector2{700, 300}),
};
const Lines lines{lines_conf};

// Layers
const std::vector<Layer> layers = {
    Layer(30),
    Layer(8, Activation::tanh),
    Layer(8, Activation::tanh),
    Layer(2, Activation::tanh),
};



int main() {
    // Setting random seed by the CPU time
    srand(time(nullptr));


    // Init the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    // loading all textures in the GPU at once
    TextureCollection::LoadAll();


    // Population
    Population population = Population()
        .setLines(lines)
        .setAnts(100)
        .setAntTexture(TextureCollection::ant)
        .setNetwork(layers, "weights.bin")
        .setPositions(Vector2{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2}, Vector2{400, 100})
        .setMovement(RADIAL_MOVE, 2)
        .setRays(30, 100);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (const auto& line : lines_conf) {
            DrawLineEx(line.start, line.end, 1.5, BLACK);
        }

        // Calculating the Ant positions
        population.act();

        // Draw the ants
        population.draw();

        DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

        EndDrawing();
    }

    TextureCollection::FreeAll();

    CloseWindow();
    return 0;
}
