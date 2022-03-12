#pragma once

#include "Object.h"

// SDL
#include "SDL2/SDL_image.h"
// STL
#include <string>


class BlockSafe {
public:
    BlockSafe(int pos_x, int pos_y, std::string image_path);
    BlockSafe(int pos_x, int pos_y, int pos_w, int pos_h, std::string image_path);

    inline int GetLeft() const {return pos.x;};
    inline int GetRight() const {return pos.x + pos.w;};
    inline int GetTop() const {return pos.y;};
    inline int GetBottom() const {return pos.y + pos.h;};
    inline SDL_Rect* GetPos() {return &pos;};
    inline SDL_Texture* GetTexture() {return texture;};

private:
    int id;
    SDL_Rect pos;
    SDL_Texture* texture;
};










