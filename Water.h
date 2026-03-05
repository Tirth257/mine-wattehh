#pragma once
#include "CONSTANTS.h"
#include "ChunkManager.h"

class Water
{
private:
    int sourceLvl = 5;
    Block *sourceBlock;
    vector<glm::vec3> kernals;
    glm::vec3 pos;

public:
    ChunkManager *chunkManager;

    Water(Block *sourceBlock, glm::vec3 pos)
    {
        this->sourceBlock = sourceBlock;
        sourceBlock->setWaterLevel(sourceLvl);
        kernals.push_back(pos);
        this->pos = pos;
    }

    void Expend()
    {
        vector<glm::vec3> new_kernals;
        vector<pair<Block*,int>> fixtures;

        for (auto &&smallWaterSMALL_source_POS : kernals)
        {
            Block *currentBlock = chunkManager->getBlockAtPos(smallWaterSMALL_source_POS);
            if (currentBlock == nullptr || currentBlock->waterLevel <= 1)
            {
                continue;
            }

            Block *down = chunkManager->getBlockAtPos(smallWaterSMALL_source_POS - glm::vec3(0, 1, 0));
            if (down->type != AIR) // solid bottom
            {
                int new_water_lvl = currentBlock->waterLevel -1;
                /*
                TODO: what if below block is water ?
                */
                if (down->type == WATER)
                {
                    continue;
                }

                for (int i = 0; i < 4; i++)
                {
                    Block *neigb = chunkManager->getBlockAtPos(smallWaterSMALL_source_POS - flowDirections[i]);

                    if (neigb->type == AIR)
                    {
                        glm::vec3 targ = smallWaterSMALL_source_POS - flowDirections[i];


                        new_kernals.push_back(targ);
                        fixtures.emplace_back(neigb,new_water_lvl);

                        chunkManager->AddChunkUpdateQuaue(targ);
                    }
                    else if (neigb->type == WATER)
                    {
                        glm::vec3 targ = smallWaterSMALL_source_POS - flowDirections[i];
                        fixtures.emplace_back(neigb,max(new_water_lvl, currentBlock->waterLevel));

                        chunkManager->AddChunkUpdateQuaue(targ);
                    }
                }
            }
            else
            {
                glm::vec3 targ = smallWaterSMALL_source_POS + glm::vec3(0, -1, 0);
                down->type = WATER;
                down->setWaterLevel(sourceLvl);
                new_kernals.push_back(targ);
                chunkManager->AddChunkUpdateQuaue(targ);
            }
        }
        for (auto &&changes  : fixtures)
        {
            changes.first->type = WATER;
            changes.first->setWaterLevel(changes.second);
        }
        
        swap(kernals, new_kernals);
    }
    ~Water();
};
