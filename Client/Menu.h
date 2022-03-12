#pragma once

#include "LTexture.h"

// STL
#include <iostream>

class Menu {
public:
    Menu();

    enum MenuType {
        NONE,
        MAIN,
        NEW_GAME,
        SETTINGS,
        LOGIN,
        REGISTER,
    };

    void Render();
    void Activated(size_t count);
    void Text();

    // NEED return only pos
    inline std::vector<LTexture*>& GetTextures() { return m_texturesMenu[m_current_menu]; }
    inline void SetShow(const MenuType x) { m_current_menu = x; };
    inline bool IsShow() { return m_current_menu; };
   // inline const bool IsActive() const { return m_active; };
   // inline void SetActive(const bool x) { m_active = x; };

private:
    MenuType m_current_menu;
    bool m_show;
   // bool m_active;
   int m_current_active = -1;
    SDL_Texture* m_texture_background;
    // unordered_map
    std::vector<std::vector<LTexture*>> m_texturesMenu;

};

/**************************************************************************/
class Texture_Other : public LTexture {
public:
    Texture_Other(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn = "");

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*****************************/
class New_Game_Button : public LTexture {
public:
    New_Game_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn);

    void Clicked() const  override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*********************************************/
class Exit_Button : public LTexture {
public:
    Exit_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn);

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*********************************************/
class Login_Button : public LTexture {
public:
    Login_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn);

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*********************************************/
class Register_Button : public LTexture {
public:
    Register_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn);

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*********************************************/
class Text_Enter_Login : public LTexture {
public:
    Text_Enter_Login(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn = "");

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/*********************************************/
class Text_Enter_Password : public LTexture {
public:
    Text_Enter_Password(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn = "");

    void Clicked() const override;
    void Activated() override;
    void Text() override;
private:
    // path?
};
/**************************************************************************************************************/













