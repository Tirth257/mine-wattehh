#pragma once

enum BlockType
{
    AIR = 0,
    DIRT,
    GRASS,
    STONE,
    WATER
};

struct Block
{
    BlockType type;
    bool isActive;
    int waterLevel = 0;
    Block()
    {
        this->type = AIR;
        this->isActive = false;
    }
    Block(BlockType type)
    {
        this->type = type;
        this->isActive = true;
    }
    void setWaterLevel(int level)
    {
        this->waterLevel = level;
    }
};
