//
// Created by yanam on 08.06.2025.
//

#ifndef TEXTURES_H
#define TEXTURES_H
#include "raylib.h"


namespace TextureCollection {
    // ---- textures ----
    inline Texture2D ant;
    inline Texture2D bee;
    inline Texture2D whiteAnt;
    inline Texture2D finishFlag;
    inline Texture2D whiteX;
    inline Texture2D star;
    inline Texture2D whiteStar;

    // ---- loaders ----
    void LoadAll();
    void FreeAll();
}



#endif //TEXTURES_H
