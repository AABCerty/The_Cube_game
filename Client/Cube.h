#pragma once

#include "Server_request.h"
#include "Object.h"
#include "BlockSafe.h"
#include "BlockTarget.h"
#include "BlockUnsafe.h"

#include <SDL2/SDL_image.h>

#include <string>
#include <vector>
#include <string_view>


class Cube {
public:
    Cube(int pos_x, int pos_y, std::string_view image_path);
    void Move(Request request);
    bool CollisionCubes(const int x, const int y);

    template <typename T>
    bool Collision(const int x, const int y, const T& other) {
        if (y + pos.h > other.GetTop() && y < other.GetBottom() && x + pos.w > other.GetLeft() && x < other.GetRight()) {
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

    // Inline
    inline int GetLeft() const {return pos.x;};
    inline int GetRight() const {return pos.x + pos.w;};
    inline int GetTop() const {return pos.y;};
    inline int GetBottom() const {return pos.y + pos.h;};
    inline SDL_Rect* GetPos() {return &pos;};
    inline SDL_Texture* GetTexture() {return texture;};
    inline void SetX(int a) {pos.x = a;};
    inline void SetY(int a) {pos.y = a;};
    inline bool IsShow() const {return m_Show;};
    inline void SetShow(bool x) {m_Show = x;};

private:
    int spd;
    SDL_Texture* texture;
    SDL_Rect pos;
    int id;
    bool m_Show;
};


