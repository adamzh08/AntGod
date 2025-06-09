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

    // ---- loaders ----
    void LoadAll();
    void FreeAll();
}



#endif //TEXTURES_H
