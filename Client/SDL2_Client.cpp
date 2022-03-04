#include "Global_var.h"
#include "boost_tcp_client.h"
#include "Server_request.h"
#include "Cube.h"
#include "BlockSafe.h"
#include "Levels.h"
#include "LTimer.h"


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#undef main

#include <iostream>
#include <vector>
#include <sstream>
#include <future>
#include <chrono>

void ListenMouseEvent();
void RenderAll();
void LoadCubes();
void ListenEvent();
void MoveIf();
void MoveCube(Request request);
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
    ip::tcp::endpoint ep(ip::address::from_string("195.161.68.193"), 2001);
    //ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    client = talk_to_server::start(service, ep, "aa", ::cubes);
    auto f = std::async([&]{service.run();});
/*******************************************************************************/
    std::chrono::time_point<std::chrono::system_clock> m_StartTime;
    std::chrono::time_point<std::chrono::system_clock> m_EndTime;
    LTimer capTimer;
    // Main
    while (run) {
        m_StartTime = std::chrono::system_clock::now();
        capTimer.start();
        ListenEvent();
        MoveIf();

        RenderAll();

        //If frame finished early
        int frameTicks = capTimer.getTicks();
        if(frameTicks < SCREEN_TICKS_PER_FRAME) {
            //Wait remaining time
            //SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
           // SDL_Delay(200);
        }
        m_EndTime = std::chrono::system_clock::now();
       // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count() * 60 << std::endl;
       // std::cout << SCREEN_TICKS_PER_FRAME << " " << frameTicks << std::endl;
   }
/*******************************************************************************/

    Quit();
    return 0;
}

void ListenMouseEvent() {
    if (current_level == 0) {
        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
            int x, y;
            SDL_GetMouseState( &x, &y );
            if (x >= 200 && x <= 400 && y >= 200 && y <= 250) {
                texturesMenu[1] = LoadImagePNG("resources/img/levels/level_0/new_game_2.png");
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    NextLevel();
                }
            } else {
                texturesMenu[1] = LoadImagePNG("resources/img/levels/level_0/new_game_1.png");
            }
            if (x >= 200 && x <= 400 && y >= 300 && y <= 350) {
                texturesMenu[2] = LoadImagePNG("resources/img/levels/level_0/exit_2.png");
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    exit(1);
                }
            } else {
                texturesMenu[2] = LoadImagePNG("resources/img/levels/level_0/exit_1.png");
            }
        }
    }
}

void ListenEvent() {
    if (flag_delay) {
        flag_delay = false;
        SDL_Delay(300);
    }
    while(SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            client->do_write_enum(Request::SERVER_QUIT);
            run = false;
        }
        ListenMouseEvent();
    }
    if (KEYS[SDL_SCANCODE_DOWN]) {
        input_down = true;
    } else {
        input_down = false;
    }
    if (KEYS[SDL_SCANCODE_UP]) {
        input_up = true;
    } else {
        input_up = false;
    }
    if (KEYS[SDL_SCANCODE_RIGHT]) {
        input_right = true;
    } else {
        input_right = false;
    }
    if (KEYS[SDL_SCANCODE_LEFT]) {
        input_left = true;
    } else {
        input_left = false;
    }
    if (KEYS[SDL_SCANCODE_N]) {
        client->do_write_enum(Request::NEXT_LEVEL);
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
                MoveCube(Request::X_PLUS_Y_PLUS);
            } else if (moveX > 0 && moveY < 0) {
                MoveCube(Request::X_PLUS_Y_MINUS);
            } else if (moveX < 0 && moveY > 0) {
                MoveCube(Request::X_MINUS_Y_PLUS);
            } else if (moveX < 0 && moveY < 0) {
                MoveCube(Request::X_MINUS_Y_MINUS);
            }
        } else if (moveX == 0) {
            if (moveY > 0) {
                MoveCube(Request::Y_PLUS);
            } else {
                MoveCube(Request::Y_MINUS);
            }
        } else {
            if (moveX > 0) {
                MoveCube(Request::X_PLUS);
            } else {
                MoveCube(Request::X_MINUS);
            }
        }
    }
}

void MoveCube(Request request) {  // Inline ?
    for (auto& cube : ::cubes) {
        cube->Move(request);
    }
    client->do_write_enum(request);
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
    for (auto& block : blocksSafe) {
        SDL_DestroyTexture(block->GetTexture());
    }
    for (auto& block : blocksTarget) {
        SDL_DestroyTexture(block->GetTexture());
    }
    for (auto& block : blocksUnsafe) {
        SDL_DestroyTexture(block->GetTexture());
    }
    for (auto& cube : ::cubes) {
        SDL_DestroyTexture(cube->GetTexture());
    }
    blocksSafe.clear();
    blocksTarget.clear();
    blocksUnsafe.clear();
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
    if (current_level == 0) {
        int x = 200;
        int y = 100;
        for (auto& tex : texturesMenu) {
            SDL_Rect pos;
            pos.x = x;
            pos.y = y;
            SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
            SDL_RenderCopy(ren, tex, NULL, &pos);
            y += 100;
        }
    }
    for (auto& block : blocksSafe) {
        SDL_RenderCopy(ren, block->GetTexture(), NULL, block->GetPos());
    }
    for (auto& block : blocksTarget) {
        SDL_RenderCopy(ren, block->GetTexture(), NULL, block->GetPos());
    }
    for (auto& block : blocksUnsafe) {
        SDL_RenderCopy(ren, block->GetTexture(), NULL, block->GetPos());
    }
    for (auto& cube : ::cubes) {
        if (cube->IsShow()) {
            SDL_RenderCopy(ren, cube->GetTexture(), NULL, cube->GetPos());
        }
    }
    SDL_RenderPresent(ren);
}










