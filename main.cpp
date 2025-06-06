#include <iostream>
#include <vector>

#include "raylib.h"
#include "raymath.h"
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

    Vector2 mousePos = GetMousePosition();
    Vector2 antPos = mousePos;
    float lerpAmount = 0.22f;

    std::vector<Layer> layers = {
        Layer(3),
        Layer(2048, Activation::gelu),
        Layer(2048, Activation::gelu),
        Layer(2, Activation::gelu),
    };

    Network net = Network(layers, "weights.bin");

    clock_t start = clock();
    float sum = 0;
    for (int i = 0; i < 100; i++) {
        std::vector<float> input = {i/100.0f, 0.2, 0.3};
        std::vector<float> output = net.feed_forward(input);
        sum += output[0];
    }
    std::cout << sum << std::endl;
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    std::cout << elapsed << std::endl;

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
