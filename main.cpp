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

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

const std::vector<Line> lines_conf = {
    Line(Vector2{500, 0}, Vector2{500, 300}),
    Line(Vector2{600, 450}, Vector2{600, 150}),
    Line(Vector2{700, 0}, Vector2{700, 300}),
};

const Lines lines(lines_conf);

const std::vector<Layer> layers = {
    Layer(30),
    Layer(16, Activation::tanh),
    Layer(16, Activation::tanh),
    Layer(16, Activation::tanh),
    Layer(2, Activation::tanh),
};

int main() {
    srand(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    Population population = Population(layers, "pop.bin", lines, 10000, 30, 30, CARTESIAN_MOVE);

    const Image ant = LoadImage("assets/ant.png");
    const Texture2D antTexture = LoadTextureFromImage(ant);

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        for (const auto& line : lines_conf) {
            DrawLineEx(line.start, line.end, 1.5, BLACK);
        }

        population.act();

        for (const auto& ant11 : population.ants) {
            DrawTextureV(antTexture, ant11.position, WHITE);
        }

        DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
