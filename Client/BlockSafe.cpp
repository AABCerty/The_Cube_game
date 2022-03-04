#include "BlockSafe.h"
#include "Global_var.h"


BlockSafe::BlockSafe(int pos_x, int pos_y, std::string image_path) : texture(LoadImagePNG(image_path)) {
    blocksSafe.push_back(this);
    id = blocksSafe.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};

BlockSafe::BlockSafe(int pos_x, int pos_y, int pos_w, int pos_h, std::string image_path) : texture(LoadImagePNG(image_path)) {
    blocksSafe.push_back(this);
    id = blocksSafe.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    pos.w = pos_w;
    pos.h = pos_h;
};









