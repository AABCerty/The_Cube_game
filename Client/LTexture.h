#pragma once

#include "SDL2/SDL_image.h"
//#include "Global_var.h"

#include <string_view>
#include <vector>
#include <iostream>


class LTexture {
public:
    LTexture(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn);

    enum TexturePush {
        NONE,
        MOUSE_ON,
    };

    void Render();

    void SetPush(const TexturePush push);
    inline int GetLeft() const {return m_pos.x;};
    inline int GetRight() const {return m_pos.x + m_pos.w;};
    inline int GetTop() const {return m_pos.y;};
    inline int GetBottom() const {return m_pos.y + m_pos.h;};

    virtual void Clicked() const  = 0;

    inline bool IsActive() { return m_active; };
    inline void SetActive(const bool x) {
        m_active = x;
        if (m_active) {
            Activated();
        }
    };

    virtual void Activated() = 0;
    virtual void Text() = 0;

private:
    // id ??
    TexturePush m_texturePush;
    std::vector<SDL_Texture*> m_texture;
    SDL_Rect m_pos;
protected:
    bool m_active = false;
};


