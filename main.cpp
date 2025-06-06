#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"
#include "gameClasses/Ant.h"
#include "neuralengine/Layer.h"
#include "neuralengine/Network.h"
#include "neuralengine/Activation.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main() {
    srand(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    const Image ant = LoadImage("assets/ant.png");
    const Texture2D antTexture = LoadTextureFromImage(ant);

    Ant testAnt(Vector2{SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});

    while (!WindowShouldClose()) {

        // logic

        testAnt.calculateFrame();

        // drawing

        BeginDrawing();

        ClearBackground(WHITE);

        DrawTextureV(antTexture, testAnt.getPos(), WHITE);

        DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
