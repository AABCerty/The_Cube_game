#include "Global_var.h"
#include "Cube.h"

// STL
#include <iostream>

Cube::Cube(const std::string_view image_path, const int pos_x, const int pos_y) : texture(LoadImagePNG(image_path)), spd(1), m_Show(false) {
    cubes.push_back(this);
    id = cubes.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};

void Cube::Move(const Request request) {
    auto [X, Y] = EnumToMove(request);
    int moveX = X * spd;
	int moveY = Y * spd;


	// Safe blocks and Cubes
    if (moveX != 0) {
        if (CollisionCubes(pos.x + moveX, pos.y) || CollisionBlocks(pos.x + moveX, pos.y, blocksSafe)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x + X, pos.y) || CollisionBlocks(pos.x + X, pos.y, blocksSafe)) {
                    break;
                }
                pos.x += X;
            }
            moveX = 0;
        }
    }
    if (moveY != 0) {
        if (CollisionCubes(pos.x, pos.y + moveY) || CollisionBlocks(pos.x, pos.y + moveY, blocksSafe)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x, pos.y + Y) || CollisionBlocks(pos.x, pos.y + Y, blocksSafe)) {
                    break;
                }
                pos.y += Y;
            }
            moveY = 0;
        }
    }
    pos.x += moveX;
    pos.y += moveY;

    // Target
    if (CollisionBlocks(cubes[0]->pos.x, cubes[0]->pos.y, blocksTarget) && CollisionBlocks(cubes[1]->pos.x, cubes[1]->pos.y, blocksTarget) && CollisionBlocks(cubes[2]->pos.x, cubes[2]->pos.y, blocksTarget)) {
        client->do_write(Request::NEXT_LEVEL);
        NextLevel();
    }
    // Unsafe
    if (CollisionBlocks<BlockUnsafe>(cubes[0]->pos.x, cubes[0]->pos.y, blocksUnsafe) || CollisionBlocks(cubes[1]->pos.x, cubes[1]->pos.y, blocksUnsafe) || CollisionBlocks(cubes[2]->pos.x, cubes[2]->pos.y, blocksUnsafe)) {
        client->do_write(Request::RESTART);
        Restart();
    }
}

/******************************************************************************/

bool Cube::CollisionCubes(const int x, const int y) const {
    for (const auto& cube : cubes) {
        if (cube->id != id) {
            if (Collision(x, y, *cube)){
                return true;
            }
        }
    }
    return false;
}


















