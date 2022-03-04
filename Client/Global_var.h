#pragma once

#include "Cube.h"
#include "BlockSafe.h"
#include "boost_tcp_client.h"
#include "BlockTarget.h"
#include "BlockUnsafe.h"

// Boost ASIO
#include <boost/asio.hpp>

// STL
#include <iostream>
#include <vector>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

extern SDL_Renderer *ren;
extern SDL_Window *win;
extern SDL_Texture* texture_background;
extern const Uint8* KEYS;
extern SDL_Event e;

extern std::vector<Cube*> cubes;
extern std::vector<BlockSafe*> blocksSafe;
extern std::vector<BlockTarget*> blocksTarget;
extern std::vector<BlockUnsafe*> blocksUnsafe;
extern std::vector<SDL_Texture*> texturesMenu;

extern boost::asio::io_service service;
extern talk_to_server::ptr client;

extern bool input_right;
extern bool input_left;
extern bool input_up;
extern bool input_down;

extern int current_level;
const int MAX_LEVELS = 3;
extern bool run;
// NEED?
extern bool flag_delay;

/**********************************************************************/
SDL_Texture* LoadImagePNG(std::string_view file);
void NextLevel();
void LoadMap(int level);
void Restart();










