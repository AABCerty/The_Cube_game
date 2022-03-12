#pragma once

#include "Server_request.h"
// SDL
#include <SDL2/SDL_image.h>
// STL
#include <vector>
#include <string_view>


class Cube {
public:
    Cube(const std::string_view image_path, const int pos_x = 0, const int pos_y = 0);
    void Move(const Request request);
    bool CollisionCubes(const int x, const int y) const;

    template <typename T>
    bool Collision(const int x, const int y, const T& other) const {
        if (y + pos.h > other.GetTop() && y < other.GetBottom() && x + pos.w > other.GetLeft() && x < other.GetRight()) {
            return true;
        }
        return false;
    }

    template <typename T>
    bool CollisionBlocks(const int x, const int y, const std::vector<T*>& vecBlocks) const {
        for (const auto& block : vecBlocks) {
            if (Collision<T>(x, y, *block)){
                return true;
            }
        }
        return false;
    };

    inline int GetLeft() const {return pos.x;};
    inline int GetRight() const {return pos.x + pos.w;};
    inline int GetTop() const {return pos.y;};
    inline int GetBottom() const {return pos.y + pos.h;};
    inline SDL_Rect* GetPos() {return &pos;};
    inline SDL_Texture* GetTexture() {return texture;};
    inline void SetX(const int a) {pos.x = a;};
    inline void SetY(const int a) {pos.y = a;};
    inline bool IsShow() const {return m_Show;};
    inline void SetShow(const bool x) {m_Show = x;};

private:
    int id;
    SDL_Rect pos;
    SDL_Texture* texture;
    int spd;
    bool m_Show;
};


