#include "raylib.h"
#include "TextureCollection.h"

void TextureCollection::LoadAll() {
    ant = LoadTexture("assets/ant.png");
    bee = LoadTexture("assets/bee.png");
}
void TextureCollection::FreeAll() {
    UnloadTexture(ant);
    UnloadTexture(bee);
}