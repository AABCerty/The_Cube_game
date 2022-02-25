#include "Global_var.h"
#include "boost_tcp_client.h"
#include "Server_request.h"
#include "Cube.h"
#include "Block.h"
#include "Levels.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include <iostream>
#include <vector>
#include <sstream>
#include <future>

void RenderAll();
void LoadCubes();
void ListenEvent();
void MoveIf();
void MoveCube(int x);
bool Init();
void Quit();

int main() {
    std::cout << "Client" << std::endl;

    // SDL Inicialisation
    if (!Init()) {
        Quit();
        system("pause");
        return 1;
    }

    LoadCubes();
    LoadMap(0);

/********************************************************************************/
//  Client - Server
    //ip::tcp::endpoint ep(ip::address::from_string("195.161.68.193"), 2001);
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    client = talk_to_server::start(service, ep, "aa", ::cubes);
    auto f = std::async([&]{service.run();});
/*******************************************************************************/
    // Main
    while (run) {
        ListenEvent();
        MoveIf();
        RenderAll();
   }
/*******************************************************************************/

    Quit();
    return 0;
}

void ListenEvent() {
    if (flag_delay) {
        flag_delay = false;
        SDL_Delay(300);
    }
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
    if (keys[SDL_SCANCODE_N]) {
        NextLevel();
        flag_delay = true;
    }
}

void MoveIf() {
    int moveX = input_right - input_left;
    int moveY = input_down - input_up;
    if (moveX != 0 || moveY != 0) {
        if (moveX != 0 && moveY != 0) {
            if (moveX > 0 && moveY > 0) {
                MoveCube(X_PLUS_Y_PLUS);
            } else if (moveX > 0 && moveY < 0) {
                MoveCube(X_PLUS_Y_MINUS);
            } else if (moveX < 0 && moveY > 0) {
                MoveCube(X_MINUS_Y_PLUS);
            } else if (moveX < 0 && moveY < 0) {
                MoveCube(X_MINUS_Y_MINUS);
            }
        } else if (moveX == 0) {
            if (moveY > 0) {
                MoveCube(Y_PLUS);
            } else {
                MoveCube(Y_MINUS);
            }
        } else {
            if (moveX > 0) {
                MoveCube(X_PLUS);
            } else {
                MoveCube(X_MINUS);
            }
        }
    }
}

void MoveCube(int x) {  // Inline ?
    for (auto& cube : ::cubes) {
        cube->Move(x);
    }
    client->do_write_enum(x);
}

bool Init() {
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
    SDL_DestroyTexture(texture_background);
    for (auto& block : blocks) {
        SDL_DestroyTexture(block->GetTexture());
    }
    for (auto& cube : ::cubes) {
        SDL_DestroyTexture(cube->GetTexture());
    }
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    IMG_Quit();
    SDL_Quit();
}

void LoadCubes() {
    new Cube(50, 50, "resources/cube_1.png");
    new Cube(300, 50, "resources/cube_2.png");
    new Cube(50, 200, "resources/cube_3.png");
}

void RenderAll() {
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, texture_background, NULL, NULL);
    for (auto& block : blocks) {
        SDL_RenderCopy(ren, block->GetTexture(), NULL, block->GetPos());
    }
    for (auto& block : blocksTarget) {
        SDL_RenderCopy(ren, block->GetTexture(), NULL, block->GetPos());
    }
    for (auto& cube : ::cubes) {
        SDL_RenderCopy(ren, cube->GetTexture(), NULL, cube->GetPos());
    }
    SDL_RenderPresent(ren);
}










