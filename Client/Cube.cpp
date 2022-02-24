#include "Cube.h"
#include "Server_request.h"

// SDL
#include <SDL2/SDL_image.h>

// STL
#include <iostream>

/*
Cube::Cube() {
}
*/

Cube::Cube(int pos_x, int pos_y, std::string image_path, SDL_Renderer* rend) : x(pos_x), y(pos_y), spd(1), render(rend), texture(LoadImagePNG(image_path, rend)) {
    pos.x = x;
    pos.y = y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};
void Cube::Move(int rrt) {
    auto move = EnumToMove(rrt);

    x += move.first * spd;
    y += move.second * spd;
    pos.x += move.first * spd;
    pos.y += move.second * spd;

}


SDL_Texture* LoadImagePNG(std::string& file, SDL_Renderer* rend){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;
    loadedImage = IMG_Load(file.c_str());
    if (loadedImage){
        texture = SDL_CreateTextureFromSurface(rend, loadedImage);
        SDL_FreeSurface(loadedImage);
    } else {
        std::cout << IMG_GetError() << std::endl;
    }
    std::cout << "OK" <<std::endl;
    return texture;
}

