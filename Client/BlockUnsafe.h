#pragma once

// SDL
#include <SDL2/SDL_image.h>

// STL
#include <string>

class BlockUnsafe {
public:
    BlockUnsafe(int pos_x, int pos_y, std::string image_path);
    BlockUnsafe(int pos_x, int pos_y, int pos_w, int pos_h, std::string image_path);

    // Inline
    int GetLeft() const {return pos.x;};
    int GetRight() const {return pos.x + pos.w;};
    int GetTop() const {return pos.y;};
    int GetBottom() const {return pos.y + pos.h;};
    SDL_Rect* GetPos() {return &pos;};
    SDL_Texture* GetTexture() {return texture;};

private:
    int id;
    SDL_Rect pos;
    SDL_Texture* texture;
};
