#pragma once

#include "Cube.h"
#include "Block.h"
#include "boost_tcp_client.h"
#include "BlockTarget.h"

// Boost ASIO
#include <boost/asio.hpp>

// STL
#include <iostream>
#include <vector>
#include <list>

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Renderer *ren;
extern SDL_Window *win;

extern std::vector<Cube*> cubes;
extern std::vector<Block*> blocks;
extern std::vector<BlockTarget*> blocksTarget;

SDL_Texture* LoadImagePNG(std::string& file);
SDL_Texture* LoadImagePNG(const char* c_string);

extern int current_level;
const int max_levels = 3;
extern bool flag_delay;
extern SDL_Texture* texture_background;

extern boost::asio::io_service service;
extern talk_to_server::ptr client;

extern SDL_Event e;
extern bool input_right;
extern bool input_left;
extern bool input_up;
extern bool input_down;
extern const Uint8* keys;
extern bool run;



void NextLevel();

void LoadMap(int level);











