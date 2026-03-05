#pragma once
#include "Camera.h"
#include "Block.h"
#include "ChunkManager.h"
#include "CONSTANTS.h"
#include "Water.h"
#include "WaterManager.h"
#include "CoreS.h"
class Player
{
private:
    Camera *cam;
    std::string name;
    ChunkManager *chunkmanager;
    WaterManager *water_manager;
    BlockType hand = WATER;
    float movementSpeed = 10;
    float mouse_Sensivity = 1.0f;

public:
    Water *myWater;
    Player(std::string name, Camera *camPointer, ChunkManager *chkmanager, WaterManager *water_manager)
    {
        this->name = name;
        cam = camPointer;
        this->chunkmanager = chkmanager;
        cam->movementSpeed = this->movementSpeed;
        cam->mouse_Sensivity = this->mouse_Sensivity;
        this->water_manager = water_manager;
        chkmanager->setReferenceCamera(cam);
    }

    // TODO: change return type to an organized Struct
    rayHit rayCast(glm::vec3 position, glm::vec3 direction, float range)
    {
        rayHit hitData;
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
                hitData.distance = d;
                hitData.hit = true;

                hitData.hitPostion = glm::vec3(x, y, z);

                hitData.hitChunk = cptr;
                hitData.hitBlock = &cptr->blocks[localX][y][localZ];
                return hitData;
            }
            hitData.prvhitPostion = glm::vec3(x, y, z);

            hitData.prevChunk = cptr;
            hitData.prevBlock = &cptr->blocks[localX][y][localZ];

            current_hit += direction * stepSize;
            d += stepSize;
        }
        return hitData;
    }
    void pickBlock()
    {
        rayHit retreted = rayCast(cam->cameraPos, cam->cameraDirection, 100);
        if (retreted.hit)
        {
            retreted.hitBlock->type = AIR;
            retreted.hitChunk->buildMesh();
            retreted.hitChunk->uploadToGPU();
        }
    }
    void PlaceBlock()
    {

        rayHit retreted = rayCast(cam->cameraPos, cam->cameraDirection, 100);

        if (retreted.hit)
        {
            if (hand == WATER)
            {

                myWater = new Water(retreted.prevBlock, retreted.prvhitPostion);
                water_manager->addWaterSource(myWater);
            }
                retreted.prevBlock->type = hand;

            retreted.prevChunk->buildMesh();
            retreted.prevChunk->uploadToGPU();
        }
    }
};
