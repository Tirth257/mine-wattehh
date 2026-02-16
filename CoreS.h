#pragma once
#include"CONSTANTS.h"
class Block;
class Chunk;

struct rayHit
{
    bool hit = false;          
    Block* hitBlock = nullptr;  // The block we hit (to destroy)
    Block* prevBlock = nullptr;// previous block (AIR)
    Chunk* hitChunk = nullptr;  // chunk in which the hit occured
    Chunk* prevChunk = nullptr; // Chunk of previous block
    glm::vec3 hitPostion;  // hit world coordinated (voxel style)   
    glm::vec3 prvhitPostion;  // prv hit world coordinated (voxel style)   
    float distance = 0.0f;   
};
