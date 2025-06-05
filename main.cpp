#include <iostream>

#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main() {
    srand(time(nullptr));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic window");

    SetTargetFPS(60);

    const Image ant = LoadImage("assets/ant.png");
    const Texture2D antTexture = LoadTextureFromImage(ant);

    Vector2 mousePos = GetMousePosition();
    Vector2 antPos = mousePos;
    float lerpAmount = 0.22f;

    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        mousePos = GetMousePosition();
        antPos = Vector2Lerp(antPos, mousePos, lerpAmount);

        DrawTextureV(antTexture, antPos, WHITE);

        DrawFPS(GetScreenWidth() - 100, GetScreenHeight() - 25);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}