#include "Global_var.h"
#include "Levels.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Renderer *ren = nullptr;
SDL_Window *win = nullptr;

std::vector<Cube*> cubes;
std::vector<Block*> blocks;
std::vector<BlockTarget*> blocksTarget;

SDL_Texture* LoadImagePNG(std::string& file){
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

SDL_Texture* LoadImagePNG(const char* c_string){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;
    loadedImage = IMG_Load(c_string);
    if (loadedImage){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
    } else {
        std::cout << IMG_GetError() << std::endl;
    }
    return texture;
}

int current_level = 1;
bool flag_delay = false;
SDL_Texture* texture_background = nullptr;



io_service service;
talk_to_server::ptr client;

SDL_Event e;
bool input_right = false;
bool input_left = false;
bool input_up = false;
bool input_down = false;
const Uint8* keys = SDL_GetKeyboardState(NULL);
bool run = true;


void NextLevel() {
    if (current_level + 1 > max_levels) {
        current_level = 1;
    } else {
        current_level++;
    }
    LoadMap(current_level-1);
}

void LoadMap(int level) {
    SDL_DestroyTexture(texture_background);
    for (auto& block : blocks) {
        SDL_DestroyTexture(block->GetTexture());
    }
    for (auto& block : blocksTarget) {
        SDL_DestroyTexture(block->GetTexture());
    }
    blocks.clear();
    blocksTarget.clear();
    texture_background = LoadImagePNG(backgrounds[level]);
    int count = 0;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 20; j++) {
            if (levels[level][i][j] == 'X'){
                new Block(j*32, i*32, "resources/block_3.png");
            }
            if (levels[level][i][j] == 'T'){
                new BlockTarget(j*32, i*32, "resources/blockTarget_1.png");
            }
            if (levels[level][i][j] == 'C'){
                cubes[count]->SetX(j*32);
                cubes[count]->SetY(i*32);
                count++;
            }
        }
    }
}






















