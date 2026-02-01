#pragma once
#include "Camera.h"
#include "Block.h"
#include "ChunkManager.h"
#include "CONSTANTS.h"
class Player
{
private:
    Camera *cam;
    std::string name;
    ChunkManager *chunkmanager;

public:
    Player(std::string name, Camera *camPointer, ChunkManager *chkmanager)
    {
        this->name = name;
        cam = camPointer;
        this->chunkmanager = chkmanager;
    }

    pair<Block *,Chunk*> rayCast(glm::vec3 position, glm::vec3 direction, float range)
    {
        float stepSize = 0.1f;
        float d = 0.0f;
        glm::vec3 current_hit = position;
        while (d < range)
        {
            int x = round(current_hit.x);
            int y = round(current_hit.y);
            int z = round(current_hit.z);

            int cx = floor((float)x / chunkSize);
            int cz = floor((float)z / chunkSize);

            int localX = mod(x, chunkSize);
            int localZ = mod(z, chunkSize);

            Chunk *cptr = this->chunkmanager->chunks[{cx, cz}];
            if (cptr->blocks[localX][y][localZ].type != AIR)
            {
                cptr->blocks[localX][y][localZ].type = AIR;
                cptr->buildMesh();
                cptr->uploadToGPU();
                return {&(cptr->blocks[localX][y][localZ]),cptr};
            }
            current_hit += direction * stepSize;
            d += stepSize;
        }
        return {nullptr,nullptr};
    }
    void pickBlock()
    {

        pair<Block *,Chunk*> ret = rayCast(cam->cameraPos, cam->cameraDirection, 100);

        if (ret.first != nullptr)
        {
            cout << "Destroyed Block" << endl;

            
            // chunk1->destroyBlock(rc,true,true);
        }
        else
        {
            cout << "Raycast missed" << endl;
        }
    }
};
