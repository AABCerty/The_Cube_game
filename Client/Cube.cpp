#include "Global_var.h"
#include "Cube.h"
#include "Server_request.h"

// SDL
#include <SDL2/SDL_image.h>

// STL
#include <iostream>

Cube::Cube(int pos_x, int pos_y, std::string_view image_path) : spd(1), texture(LoadImagePNG(image_path)), m_Show(false) {
    cubes.push_back(this);
    id = cubes.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};

void Cube::Move(Request request) {
    auto move = EnumToMove(request);
    int moveX = move.first * spd;
	int moveY = move.second * spd;


	// Safe blocks and Cubes
    if (moveX != 0) {
        if (CollisionCubes(pos.x + moveX, pos.y) || CollisionBlocks(pos.x + moveX, pos.y, blocksSafe)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x + move.first, pos.y) || CollisionBlocks(pos.x + move.first, pos.y, blocksSafe)) {
                    break;
                }
                pos.x += move.first;
            }
            moveX = 0;
        }
    }
    if (moveY != 0) {
        if (CollisionCubes(pos.x, pos.y + moveY) || CollisionBlocks(pos.x, pos.y + moveY, blocksSafe)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x, pos.y + move.second) || CollisionBlocks(pos.x, pos.y + move.second, blocksSafe)) {
                    break;
                }
                pos.y += move.second;
            }
            moveY = 0;
        }
    }
    pos.x += moveX;
    pos.y += moveY;

    // Target
    if (CollisionBlocks(cubes[0]->pos.x, cubes[0]->pos.y, blocksTarget) && CollisionBlocks(cubes[1]->pos.x, cubes[1]->pos.y, blocksTarget) && CollisionBlocks(cubes[2]->pos.x, cubes[2]->pos.y, blocksTarget)) {
        client->do_write_enum(Request::NEXT_LEVEL);
        NextLevel();
    }
    // Unsafe
    if (CollisionBlocks<BlockUnsafe>(cubes[0]->pos.x, cubes[0]->pos.y, blocksUnsafe) || CollisionBlocks(cubes[1]->pos.x, cubes[1]->pos.y, blocksUnsafe) || CollisionBlocks(cubes[2]->pos.x, cubes[2]->pos.y, blocksUnsafe)) {
        client->do_write_enum(Request::RESTART);
        Restart();
    }
}

/******************************************************************************/

bool Cube::CollisionCubes(const int x, const int y) {
    for (const auto& cube : cubes) {
        if (cube->id != id) {
            if (Collision(x, y, *cube)){
                return true;
            }
        }
    }
    return false;
}


















