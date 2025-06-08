#include "Textures.h"

namespace Textures {
    Texture2D ant;

    void LoadAll() {
        ant = LoadTexture("assets/ant.png");
    }
    void FreeAll() {
        UnloadTexture(ant);
    }
}