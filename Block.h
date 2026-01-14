#pragma once

enum BlockType{
    AIR = 0,
    DIRT,
    GRASS,
    STONE
};

struct Block
{
    BlockType type;
    bool isActive;
};
