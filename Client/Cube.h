#pragma once

#include "Object.h"
#include "Block.h"
#include "BlockTarget.h"
#include "BlockUnsafe.h"

#include <SDL2/SDL_image.h>

#include <string>
#include <vector>


class Cube {
public:
    // NEED image_path with &
    Cube(int pos_x, int pos_y, std::string image_path);
    void Move(int rr);

    template <typename T>
    bool Collision(const int x, const int y, const T& other) {
        // NEED to rewrite
        if (y + pos.h <= other.GetTop() || y >= other.GetBottom() || x + pos.w <= other.GetLeft() || x >= other.GetRight()) {
            return false;
        } else {
            return true;
        }
        return false;
    }

    template <typename T>
    bool CollisionBlocks(const int x, const int y, const std::vector<T*>& vecBlocks) {
        for (const auto& block : vecBlocks) {
            if (Collision<T>(x, y, *block)){
                return true;
            }
        }
        return false;
    };


    bool Collision(const int x, const int y, const Cube& other);
    bool Collision(const int x, const int y, const Block& other);
    bool Collision(const int x, const int y, const BlockTarget& other);
    bool Collision(const int x, const int y, const BlockUnsafe& other);
    bool CollisionCubes(const int x, const int y);
    bool CollisionBlocks(const int x, const int y);
    bool CollisionBlocksTarget(const int x, const int y);
    bool CollisionBlocksUnsafe(const int x, const int y);

    // Inline
    int GetLeft() const {return pos.x;};
    int GetRight() const {return pos.x + pos.w;};
    int GetTop() const {return pos.y;};
    int GetBottom() const {return pos.y + pos.h;};
    SDL_Rect* GetPos() {return &pos;};
    SDL_Texture* GetTexture() {return texture;};
    void SetX(int a) {pos.x = a;};
    void SetY(int a) {pos.y = a;};
    bool IsShow() const {return m_Show;};
    void SetShow(bool x) {m_Show = x;};

private:
    int spd;
    SDL_Texture* texture;
    SDL_Rect pos;
    int id;
    bool m_Show;
};


