#pragma once

#include "Object.h"

#include <SDL2/SDL_image.h>

#include <string>


class Cube {
public:
    //Cube();
    // NEED image_path with &
    Cube(int pos_x, int pos_y, std::string image_path, SDL_Renderer* rend);
    void Move(int rr);

//private:
    int x;
    int y;
    int spd;
    SDL_Renderer* render;
    SDL_Texture* texture;
    SDL_Rect pos;

};


SDL_Texture* LoadImagePNG(std::string& file, SDL_Renderer* rend);
