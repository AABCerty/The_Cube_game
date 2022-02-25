#include "Global_var.h"
#include "Cube.h"
#include "Server_request.h"

// SDL
#include <SDL2/SDL_image.h>

// STL
#include <iostream>

Cube::Cube(int pos_x, int pos_y, std::string image_path) : spd(1), texture(LoadImagePNG(image_path)) {
    cubes.push_back(this);
    id = cubes.size() - 1;
    pos.x = pos_x;
    pos.y = pos_y;
    SDL_QueryTexture(texture, NULL, NULL, &pos.w, &pos.h);
};

void Cube::Move(int rrt) {
    auto move = EnumToMove(rrt);
    int moveX = move.first * spd;
	int moveY = move.second * spd;
	int finishedNum = 0;

    if (moveX != 0) {
        if (CollisionCubes(pos.x + moveX, pos.y) || CollisionBlocks(pos.x + moveX, pos.y)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x + move.first, pos.y) || CollisionBlocks(pos.x + move.first, pos.y)) {
                    break;
                }
                pos.x += move.first;
            }
            moveX = 0;
        }
    }
    if (moveY != 0) {
        if (CollisionCubes(pos.x, pos.y + moveY) || CollisionBlocks(pos.x, pos.y + moveY)) {
            for (int i = 0; i < spd; i++) {
                if (CollisionCubes(pos.x, pos.y + move.second) || CollisionBlocks(pos.x, pos.y + move.second)) {
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
    if (CollisionBlocksTarget(cubes[0]->pos.x, cubes[0]->pos.y) && CollisionBlocksTarget(cubes[1]->pos.x, cubes[1]->pos.y) && CollisionBlocksTarget(cubes[2]->pos.x, cubes[2]->pos.y)) {
        NextLevel();
    }

    /*
    for (int i = 0; i < cubes.size(); i++) {
        if (CollisionBlocksTarget(pos.x, pos.y)){
            finishedNum++;
        }
    }
    if (finishedNum == cubes.size()) {
        NextLevel();
    }
    */
}

bool Cube::Collision(const int x, const int y, const Cube& other) {
    // NEED to rewrite
    if (y + pos.h <= other.GetTop() || y >= other.GetBottom() || x + pos.w <= other.GetLeft() || x >= other.GetRight()) {
        return false;
    } else {
        return true;
    }
    return false;
}

bool Cube::Collision(const int x, const int y, const Block& other) {
    // NEED to rewrite
    if (y + pos.h <= other.GetTop() || y >= other.GetBottom() || x + pos.w <= other.GetLeft() || x >= other.GetRight()) {
        return false;
    } else {
        return true;
    }
    return false;
}
bool Cube::Collision(const int x, const int y, const BlockTarget& other) {
    // NEED to rewrite
    if (y + pos.h <= other.GetTop() || y >= other.GetBottom() || x + pos.w <= other.GetLeft() || x >= other.GetRight()) {
        return false;
    } else {
        return true;
    }
    return false;
}
bool Cube::CollisionBlocks(const int x, const int y) {
    for (const auto& block : blocks) {
        if (Collision(x, y, *block)){
            return true;
        }
    }
    return false;
}

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
bool Cube::CollisionBlocksTarget(const int x, const int y) {
    for (const auto& block : blocksTarget) {
        if (Collision(x, y, *block)){
            return true;
        }
    }
    return false;
}


