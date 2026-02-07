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
    BlockType hand = GRASS;
public:
    Player(std::string name, Camera *camPointer, ChunkManager *chkmanager)
    {
        this->name = name;
        cam = camPointer;
        this->chunkmanager = chkmanager;
    }

    vector<void *> rayCast(glm::vec3 position, glm::vec3 direction, float range)
    {
        float stepSize = 0.1f;
        float d = 0.0f;
        glm::vec3 current_hit = position;
        Block *prv = nullptr;
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
                return {&(cptr->blocks[localX][y][localZ]), prv, cptr};
            }
            prv = &cptr->blocks[localX][y][localZ];
            current_hit += direction * stepSize;
            d += stepSize;
        }
        return {nullptr, nullptr, nullptr};
    }
    void pickBlock()
    {

        vector<void *> ret = rayCast(cam->cameraPos, cam->cameraDirection, 100);
        Block *b = (Block *)ret[0];
        Chunk *h_chunk = (Chunk *)ret[2];
        if (ret[0] != nullptr)
        {
            b->type = AIR;
            h_chunk->buildMesh();
            h_chunk->uploadToGPU();
            cout << "Destroyed Block" << endl;

            // chunk1->destroyBlock(rc,true,true);
        }
        else
        {
            cout << "Raycast missed" << endl;
        }
    }
    void PlaceBlock()
    {

        vector<void *> ret = rayCast(cam->cameraPos, cam->cameraDirection, 100);

        Block *b = (Block *)ret[1];
        Chunk *h_chunk = (Chunk *)ret[2];
        if (ret[1] != nullptr)
        {
            b->type = hand;
            h_chunk->buildMesh();
            h_chunk->uploadToGPU();
            cout << "Placed Block" << endl;

        }
        else
        {
            cout << "Raycast missed" << endl;
        }
    }
};
