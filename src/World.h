#pragma once
#include "WaterManager.h"
#include "ChunkManager.h"
class World
{
public:
    ChunkManager *chunk_manager;
    WaterManager *water_manager;
    World(ChunkManager *chunk_manager, WaterManager *water_manager)
    {
        this->chunk_manager = chunk_manager;
        this->water_manager = water_manager;
    }
    void updateWater()
    {
        water_manager->updateAllWaters();
    }
}