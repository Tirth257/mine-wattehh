#pragma once
#include"Camera.h"
#include"Block.h"
#include<bits/stdc++.h>
#include"ChunkManager.h"
class Player
{
private:
    Camera *cam;
    std::string name;
    ChunkManager* chunkmanager;
    
public:
    Chunk* chunk1;
    Player(std::string name,Camera *camPointer){
        this->name = name;
        cam = camPointer;
    }
    
    Block* rayCast(glm::vec3 position,glm::vec3 direction,float range){
        float d = 0;
        Block* hitBlock = nullptr;
        glm::vec3 current_hit = position+direction;
        while (d < range)
        {
            int x= round(current_hit.x); 
            int y= round(current_hit.y); 
            int z= round(current_hit.z); 
            
            if (chunk1->blocks[x][y][z].type != AIR)
            {
                return &(chunk1->blocks[x][y][z]);
            }
            
            d++;
        }
        return nullptr;
    }
    void pickBlock(){
        Block* rc = rayCast(cam->cameraPos,cam->cameraDirection,100);
        return;
        if (rc != nullptr)
        {
            cout<<"Destroyed Block" << endl;

            chunk1->destroyBlock(rc,true,true);
        }
        else
        {
            cout << "Raycast missed" << endl;
        }
                

    }
};
