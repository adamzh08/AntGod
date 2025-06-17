#include "raylib.h"
#include "TextureCollection.h"

void TextureCollection::LoadAll() {
    ant = LoadTexture("assets/ant.png");
    bee = LoadTexture("assets/bee.png");
    whiteAnt = LoadTexture("assets/WhiteAnt.png");
    finishFlag = LoadTexture("assets/finishFlag.png");
    whiteX = LoadTexture("assets/whiteX.png");
}
void TextureCollection::FreeAll() {
    UnloadTexture(ant);
    UnloadTexture(bee);
    UnloadTexture(whiteAnt);
    UnloadTexture(finishFlag);
    UnloadTexture(whiteX);
}