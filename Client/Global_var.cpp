#include "Global_var.h"
#include "Levels.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

SDL_Renderer *ren = nullptr;
SDL_Window *win = nullptr;
SDL_Texture* texture_background = nullptr;
const Uint8* KEYS = SDL_GetKeyboardState(NULL);
SDL_Event e;

std::vector<Cube*> cubes;
std::vector<BlockSafe*> blocksSafe;
std::vector<BlockTarget*> blocksTarget;
std::vector<BlockUnsafe*> blocksUnsafe;
std::vector<SDL_Texture*> texturesMenu;

io_service service;
talk_to_server::ptr client;

bool input_right = false;
bool input_left = false;
bool input_up = false;
bool input_down = false;

int current_level = 0;
bool run = true;
// NEED?
bool flag_delay = false;

/******************************************************************************/
SDL_Texture* LoadImagePNG(std::string_view file){
    SDL_Surface *loadedImage = nullptr;
    SDL_Texture *texture = nullptr;
    loadedImage = IMG_Load(file.data());
    if (loadedImage){
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
    } else {
        std::cout << IMG_GetError() << std::endl;
    }
    return texture;
}

void NextLevel() {
    if (current_level + 1 > MAX_LEVELS) {
        current_level = 1;
    } else {
        current_level++;
    }
    LoadMap(current_level);

}

void LoadMap(int level) {
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
    blocksSafe.clear();
    blocksTarget.clear();
    blocksUnsafe.clear();
    if (level == 0) {
        for (size_t i = 0; i < cubes.size(); i++) {
            cubes[i]->SetShow(false);
        }
        texture_background = LoadImagePNG("resources/img/levels/level_0/level_0_bg.png");
        // NEED to REWORK class menu;
        if (texturesMenu.size() == 0) {
            texturesMenu.push_back(LoadImagePNG("resources/img/levels/level_0/menu_1.png"));
            texturesMenu.push_back(LoadImagePNG("resources/img/levels/level_0/new_game_1.png"));
            texturesMenu.push_back(LoadImagePNG("resources/img/levels/level_0/exit_1.png"));
        }
    } else {
        texture_background = LoadImagePNG(backgrounds[level-1]);
        int cubeNumber = 0;
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 20; j++) {
                // switch
                if (levels_map[level-1][i][j] == 'X') {
                    new BlockSafe(j*32, i*32, "resources/block_3.png");
                } else if (levels_map[level-1][i][j] == 'T') {
                    new BlockTarget(j*32, i*32, "resources/blockTarget_1.png");
                } else if (levels_map[level-1][i][j] == 'U') {
                    new BlockUnsafe(j*32, i*32, "resources/blockUnsafe_1.png");
                } else if (levels_map[level-1][i][j] == 'C') {
                    cubes[cubeNumber]->SetShow(true);
                    cubes[cubeNumber]->SetX(j*32);
                    cubes[cubeNumber]->SetY(i*32);
                    cubeNumber++;
                }
            }
        }
    }
}

void Restart() {
    current_level = 0;
    LoadMap(0);
}




















