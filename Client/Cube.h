#pragma once

#include "Object.h"
#include "Block.h"
#include "BlockTarget.h"

#include <SDL2/SDL_image.h>

#include <string>


class Cube {
public:
    // NEED image_path with &
    Cube(int pos_x, int pos_y, std::string image_path);
    void Move(int rr);

    bool Collision(const int x, const int y, const Cube& other);
    bool Collision(const int x, const int y, const Block& other);
    bool Collision(const int x, const int y, const BlockTarget& other);
    bool CollisionCubes(const int x, const int y);
    bool CollisionBlocks(const int x, const int y);
    bool CollisionBlocksTarget(const int x, const int y);

    // Inline
    int GetLeft() const {return pos.x;};
    int GetRight() const {return pos.x + pos.w;};
    int GetTop() const {return pos.y;};
    int GetBottom() const {return pos.y + pos.h;};
    SDL_Rect* GetPos() {return &pos;};
    SDL_Texture* GetTexture() {return texture;};
    void SetX(int a) {pos.x = a;};
    void SetY(int a) {pos.y = a;};

private:
    int spd;
    SDL_Texture* texture;
    SDL_Rect pos;
    int id;
};


