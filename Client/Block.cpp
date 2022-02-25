#include "Block.h"
#include "Global_var.h"


Block::Block(int pos_x, int pos_y, std::string image_path) : texture(LoadImagePNG(image_path)) {
    blocks.push_back(this);
    id = blocks.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};

Block::Block(int pos_x, int pos_y, int pos_w, int pos_h, std::string image_path) : texture(LoadImagePNG(image_path)) {
    blocks.push_back(this);
    id = blocks.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    pos.w = pos_w;
    pos.h = pos_h;
};









