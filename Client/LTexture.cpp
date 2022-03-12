#include "LTexture.h"
#include "Global_var.h"

LTexture::LTexture(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn): m_texturePush(NONE)
                                                                                                                            , m_pos(pos_x, pos_y, 0, 0) {
    m_texture.push_back(LoadImagePNG(path_None));

    if (path_MouseOn != "") {
        m_texture.push_back(LoadImagePNG(path_MouseOn));
    } else {
        m_texture.push_back(nullptr);
    }

    SDL_QueryTexture(m_texture[0], NULL, NULL, &m_pos.w, &m_pos.h);
}

void LTexture::Render() {
    SDL_RenderCopy(ren, m_texture[m_texturePush], NULL, &m_pos);
}

void LTexture::SetPush(const TexturePush push) {
    if (m_texture[MOUSE_ON]) {
        m_texturePush = push;
    }
};
















