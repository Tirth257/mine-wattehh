#pragma once
#include "CONSTANTS.h"
#include "ChunkManager.h"
#include "CoreS.h"
class WaterManager
{
private:
    /* data */
    const int sourceLvl = 5;
    ChunkManager *chunkmanager;

public:
    vector<glm::ivec3> kernals;

    WaterManager(ChunkManager *chunkManager)
    {
        this->chunkmanager = chunkManager;
    }

    void updateAllWaters()
    {

        this->Expend();
        chunkmanager->updateBatchChunk();
    }
    void Expend()
    {
        unordered_set<glm::ivec3, Vec3Hash> visited;

        vector<glm::ivec3> new_kernals;
        vector<pair<Block *, int>> fixtures;
        for (auto &&smallWaterSMALL_source_POS : kernals)
        {
            Block *currentBlock = chunkmanager->getBlockAtPos(smallWaterSMALL_source_POS);
            if (currentBlock == nullptr)
            {
                continue;
            }

            Block *down = chunkmanager->getBlockAtPos(smallWaterSMALL_source_POS - glm::ivec3(0, 1, 0));
            if (down->type != AIR) // solid bottom
            {
                int new_water_lvl = currentBlock->waterLevel - 1;

                if (new_water_lvl == 0)
                {
                    continue;
                }
                /*
                TODO: what if below block is water ?
                */
                if (down->type == WATER)
                {
                    glm::ivec3 targ = smallWaterSMALL_source_POS - glm::ivec3(0, 1, 0);
                    if (visited.insert(targ).second)
                    {
                        new_kernals.push_back(targ);
                        fixtures.emplace_back(down, sourceLvl);
                        chunkmanager->AddChunkUpdateQuaue(targ);
                    }
                }

                for (int i = 0; i < 4; i++)
                {
                    Block *neigb = chunkmanager->getBlockAtPos(smallWaterSMALL_source_POS - flowDirections[i]);

                    if (neigb->type == AIR)
                    {
                        glm::ivec3 targ = smallWaterSMALL_source_POS - flowDirections[i];

                        if (visited.insert(targ).second)
                        {
                            new_kernals.push_back(targ);
                            fixtures.emplace_back(neigb, new_water_lvl);
                            chunkmanager->AddChunkUpdateQuaue(targ);
                        }
                    }
                    else if (neigb->type == WATER)
                    {
                        glm::ivec3 targ = smallWaterSMALL_source_POS - flowDirections[i];
                        if (new_water_lvl > neigb->waterLevel)
                        {
                            fixtures.emplace_back(neigb, max(new_water_lvl, neigb->waterLevel));
                            chunkmanager->AddChunkUpdateQuaue(targ);
                        }
                    }
                }
            }
            else
            {
                glm::ivec3 targ = smallWaterSMALL_source_POS + glm::ivec3(0, -1, 0);
                down->type = WATER;
                down->setWaterLevel(sourceLvl);
                if (visited.insert(targ).second)
                {
                    new_kernals.push_back(targ);
                    chunkmanager->AddChunkUpdateQuaue(targ);
                }
            }
        }
        for (auto &&changes : fixtures)
        {
            changes.first->type = WATER;
            changes.first->setWaterLevel(changes.second);
        }
        cout << "Kernals size: " << kernals.size() << "New kernal size: " << new_kernals.size() << endl;

        swap(kernals, new_kernals);
    }
    void addWater(rayHit &rayhit)
    {
        this->kernals.push_back(rayhit.prvhitPostion);
        rayhit.prevBlock->setWaterLevel(sourceLvl);
    }
    void changeAround(glm::ivec3 pos)
    {

        Block *up = chunkmanager->getBlockAtPos(pos + glm::ivec3(0, 1, 0));
        if (up->type == WATER)
        {

            glm::ivec3 targ = pos + glm::ivec3(0, 1, 0);
            kernals.push_back(targ);
        }

        for (int i = 0; i < 4; i++)
        {
            Block *neigb = chunkmanager->getBlockAtPos(pos - flowDirections[i]);

            if (neigb->type == WATER)
            {
                glm::ivec3 targ = pos - flowDirections[i];
                kernals.push_back(targ);
            }
        }
    }

    void removeWaterSource();
};
