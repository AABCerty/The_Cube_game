#include "Menu.h"
#include "Global_var.h"

Menu::Menu() : m_current_menu(MAIN), m_texturesMenu(10) { // 10 REWORK
    m_texture_background = LoadImagePNG("resources/img/levels/level_0/level_0_bg.png");

    m_texturesMenu[MAIN].emplace_back(new Texture_Other(200, 100, "resources/img/levels/level_0/menu_1.png"));
    m_texturesMenu[MAIN].emplace_back(new New_Game_Button(200, 200, "resources/img/levels/level_0/new_game_1.png", "resources/img/levels/level_0/new_game_2.png"));
    m_texturesMenu[MAIN].push_back(new Exit_Button(200, 300, "resources/img/levels/level_0/exit_1.png", "resources/img/levels/level_0/exit_2.png"));

    m_texturesMenu[NEW_GAME].push_back(new Login_Button(150, 150, "resources/img/levels/level_0/login_1.png", "resources/img/levels/level_0/login_2.png"));
    m_texturesMenu[NEW_GAME].push_back(new Register_Button(150, 250, "resources/img/levels/level_0/register_1.png", "resources/img/levels/level_0/register_2.png"));

    m_texturesMenu[LOGIN].push_back(new Texture_Other(50, 100, "resources/img/levels/level_0/text_login_1.png"));
    m_texturesMenu[LOGIN].push_back(new Text_Enter_Login(350, 100, "resources/img/levels/level_0/text_empty_1.png"));
    m_texturesMenu[LOGIN].push_back(new Texture_Other(50, 200, "resources/img/levels/level_0/text_password_1.png"));
    m_texturesMenu[LOGIN].push_back(new Text_Enter_Password(350, 200, "resources/img/levels/level_0/text_empty_1.png"));

};

void Menu::Render() {
    // m_show
    if (current_level == 0) {
        for (size_t i = 0; i < cubes.size(); i++) {
            cubes[i]->SetShow(false);
        }
        SDL_RenderCopy(ren, m_texture_background, NULL, NULL);
        for (auto& texture : m_texturesMenu[m_current_menu]) {
            texture->Render();
        }
    }
};
void Menu::Activated(size_t count) {
    m_current_active = count;
    for (size_t i = 0; i < m_texturesMenu[m_current_menu].size(); i++) {
        if (i == count) {
            m_texturesMenu[m_current_menu][i]->SetActive(true);
        } else {
            m_texturesMenu[m_current_menu][i]->SetActive(false);
        }
    }

}
void Menu::Text() {
    m_texturesMenu[m_current_menu][m_current_active]->Text();
}
/******************************************************************************/
Texture_Other::Texture_Other(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Texture_Other::Clicked() const {
};
void Texture_Other::Activated() {
    isTextListen = false;
};
void Texture_Other::Text() {
};
/**************************/
New_Game_Button::New_Game_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};
void New_Game_Button::Clicked() const {
    menu->SetShow(Menu::NEW_GAME);
}
void New_Game_Button::Activated() {
    isTextListen = false;
};
void New_Game_Button::Text() {
};
/***************************/
Exit_Button::Exit_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Exit_Button::Clicked() const {
    exit(1);
}
void Exit_Button::Activated() {
    isTextListen = false;
};
void Exit_Button::Text() {
};
/***************************/
Login_Button::Login_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Login_Button::Clicked() const {
    menu->SetShow(Menu::LOGIN);
}
void Login_Button::Activated() {
    isTextListen = false;
};
void Login_Button::Text() {
};
/***************************/
Register_Button::Register_Button(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Register_Button::Clicked() const {
    menu->SetShow(Menu::REGISTER);
}
void Register_Button::Activated() {
    isTextListen = false;
};
void Register_Button::Text() {
};
/***************************/
Text_Enter_Login::Text_Enter_Login(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Text_Enter_Login::Clicked() const {

}
void Text_Enter_Login::Activated() {
    std::cout << "Text_enter_Login Activated" << std::endl;
    isTextListen = true;
};
void Text_Enter_Login::Text() {
    std::cout << textListen << std::endl;
    client->do_write(Request::LOGIN, textListen);
    client->s_username = textListen;
    textListen.clear();
};
/***************************/
Text_Enter_Password::Text_Enter_Password(const int pos_x, const int pos_y, const std::string_view path_None, const std::string_view path_MouseOn)
: LTexture(pos_x, pos_y, path_None, path_MouseOn) {

};

void Text_Enter_Password::Clicked() const {

}
void Text_Enter_Password::Activated() {
    std::cout << "Text_enter_Password Activated" << std::endl;
    isTextListen = false;
};
void Text_Enter_Password::Text() {
};
/**************************************************************************/












