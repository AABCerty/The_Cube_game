#include "boost_tcp_client.h"
#include "Server_request.h"
#include "Cube.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/core/noncopyable.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include <iostream>
#include <vector>
#include <sstream>
#include <future>
//195.161.68.193

// SDL
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Window *win = nullptr;
SDL_Renderer *ren = nullptr;

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend);
SDL_Texture* LoadImagePNG(std::string file);
void Quit();
bool init();

// boost asio
using namespace boost::asio;

io_service service;

int main() {
    std::cout << "Client" << std::endl;

    // SDL Inicialisation
    if (!init()) {
        Quit();
        system("pause");
        return 1;
    }

    SDL_Texture* texture_bg_1 = LoadImagePNG("resources/background_1.png");
  //  SDL_Texture* texture_cube_1 = LoadImagePNG("resources/cube_1.png");
  //  SDL_Texture* texture_cube_2 = LoadImagePNG("resources/cube_2.png");

    Cube cube_1(50, 50, "resources/cube_1.png", ren);
    Cube cube_2(300, 50, "resources/cube_2.png", ren);
    Cube cube_3(50, 200, "resources/cube_3.png", ren);
    std::vector<Cube*> cubes;
    cubes.push_back(&cube_1);
    cubes.push_back(&cube_2);
    cubes.push_back(&cube_3);

//  Client - Server
    //ip::tcp::endpoint ep(ip::address::from_string("195.161.68.193"), 2001);
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    auto client = talk_to_server::start(service, ep, "aa", cubes);
    auto f = std::async([&]{service.run();});





    SDL_Event e;

    bool input_right = false;
    bool input_left = false;
    bool input_up = false;
    bool input_down = false;
    int moveX = 0;
    int moveY = 0;
    int player_spd = 1;
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    bool run = true;
    while (run) {

        while(SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                client->do_write_enum(SERVER_QUIT);
                run = false;
            }
        }
        if (keys[SDL_SCANCODE_DOWN]) {
            input_down = true;
        } else {
            input_down = false;
        }
        if (keys[SDL_SCANCODE_UP]) {
            input_up = true;
        } else {
            input_up = false;
        }
        if (keys[SDL_SCANCODE_RIGHT]) {
            input_right = true;
        } else {
            input_right = false;
        }
        if (keys[SDL_SCANCODE_LEFT]) {
            input_left = true;
        } else {
            input_left = false;
        }
        moveX = (input_right - input_left) * player_spd;
        moveY = (input_down - input_up) * player_spd;
        if (moveX != 0 || moveY != 0) {
            if (moveX != 0 && moveY != 0) {
                if (moveX > 0 && moveY > 0) {
                    for (auto& x : cubes) {
                        x->Move(X_PLUS_Y_PLUS);
                    }
                    client->do_write_enum(X_PLUS_Y_PLUS);
                } else if (moveX > 0 && moveY < 0) {
                    for (auto& x : cubes) {
                        x->Move(X_PLUS_Y_MINUS);
                    }
                    client->do_write_enum(X_PLUS_Y_MINUS);
                } else if (moveX < 0 && moveY > 0) {
                    for (auto& x : cubes) {
                        x->Move(X_MINUS_Y_PLUS);
                    }
                    client->do_write_enum(X_MINUS_Y_PLUS);
                } else if (moveX < 0 && moveY < 0) {
                    for (auto& x : cubes) {
                        x->Move(X_MINUS_Y_MINUS);
                    }
                    client->do_write_enum(X_MINUS_Y_MINUS);
                }
            } else if (moveX == 0) {
                if (moveY > 0) {
                    for (auto& x : cubes) {
                        x->Move(Y_PLUS);
                    }
                    client->do_write_enum(Y_PLUS);
                } else {
                    for (auto& x : cubes) {
                        x->Move(Y_MINUS);
                    }
                    client->do_write_enum(Y_MINUS);
                }
            } else {
                if (moveX > 0) {
                    for (auto& x : cubes) {
                        x->Move(X_PLUS);
                    }
                    client->do_write_enum(X_PLUS);
                } else {
                    for (auto& x : cubes) {
                        x->Move(X_MINUS);
                    }
                    client->do_write_enum(X_MINUS);
                }
            }
        }

        SDL_RenderClear(ren);

        SDL_RenderCopy(ren, texture_bg_1, NULL, NULL);
      //  ApplySurface(50+client->m_x, 50+client->m_y, texture_cube_1, ren);
      //  ApplySurface(300+client->m_x, 50+client->m_y, texture_cube_2, ren);
       // ApplySurface(cube_3.x, cube_3.y, cube_3.texture, ren);
        for (auto& cube : cubes) {
            SDL_RenderCopy(ren, cube->texture, NULL, &cube->pos);
        }
       // SDL_RenderCopy(ren, cube_3.texture, NULL, &cube_3.pos);
        SDL_RenderPresent(ren);

       // boost::this_thread::sleep( boost::posix_time::millisec(10));
   }

//    SDL_DestroyTexture(texture_cube_2);
  //  SDL_DestroyTexture(texture_cube_1);
    SDL_DestroyTexture(texture_bg_1);
    Quit();

    std::cout << "END" << std::endl;

    return 0;
}




bool init() {
    bool ok = true;

    // Init SDL
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cout << "Can't init SDL: " << SDL_GetError() << std::endl;
    }

    // Init PNG
    int flags = IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        std::cout << "Can't init PNG: " << IMG_GetError() << std::endl;
    }



    // Window
    win = SDL_CreateWindow("Примитивы", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!win) {
        std::cout << "Can't create window: " << SDL_GetError() << std::endl;
        ok = false;
    }

    // Render
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        std::cout << "Can't create renderer: " << SDL_GetError() << std::endl;
        ok = false;
    }

    return ok;
}

void Quit() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* LoadImagePNG(std::string file){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;
    loadedImage = IMG_Load(file.c_str());
    if (loadedImage){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
    } else {
        std::cout << IMG_GetError() << std::endl;
    }
    return texture;
}

void ApplySurface(int x, int y, SDL_Texture *tex, SDL_Renderer *rend){
   SDL_Rect pos;
   pos.x = x;
   pos.y = y;
   SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
   SDL_RenderCopy(rend, tex, NULL, &pos);
}

