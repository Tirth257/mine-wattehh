#pragma once
#include "CONSTANTS.h"
#include "Water.h"
#include "ChunkManager.h"

class WaterManager
{
private:
    /* data */
    unordered_set<Water *> waterSources;
    ChunkManager *chunkmanager;

public:
    WaterManager(ChunkManager *chunkManager)
    {
        this->chunkmanager = chunkManager;
    }

    void updateAllWaters()
    {
        for (auto &&i : waterSources)
        {
            i->Expend();
        }
        chunkmanager->updateBatchChunk();
    }
    void addWaterSource(Water *water)
    {
        waterSources.insert(water);
        water->chunkManager = this->chunkmanager;
    }
    void removeWaterSource();
};
