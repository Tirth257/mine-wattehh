#pragma once
#include "Chunk.h"
#include "CONSTANTS.h"
class ChunkManager
{
private:
    struct PairHash
    {
        size_t operator()(const std::pair<int, int> &p) const noexcept
        {
            size_t h1 = std::hash<int>{}(p.first);
            size_t h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };
    Camera *cam;
    int squareSize;

public:
    std::unordered_map<pair<int, int>, Chunk *, PairHash> chunks;
    std::unordered_set<Chunk*> updatedChunks_indx;

    ChunkManager(int squareSize)
    {
        this->squareSize = squareSize;
    }
    void genMapping()
    {
        for (int i = -squareSize / 2; i < squareSize / 2; i++)
        {
            for (int j = -squareSize / 2; j < squareSize / 2; j++)
            {
                Chunk *tempChunk = new Chunk({i, j});
                tempChunk->GenerateTerrain();
                tempChunk->buildMesh();
                tempChunk->uploadToGPU();
                chunks[{i, j}] = tempChunk;
            }
        }
    }
    void draw()
    {

        for (int i = -squareSize / 2; i < squareSize / 2; i++)
        {
            for (int j = -squareSize / 2; j < squareSize / 2; j++)
            {
                chunks[{i, j}]->draw();
            }
        }
    }

    void updateBatchChunk(){
        for (auto &&i : updatedChunks_indx)
        {
            i->buildMesh();
            i->uploadToGPU();
            
        }
        updatedChunks_indx.clear();
        
        
    }
    Block *getBlockAtPos(glm::vec3 pos)
    {
        int x = round(pos[0]);
        int y = round(pos[1]);
        int z = round(pos[2]);

        int cx = floor((float)x / chunkSize);
        int cz = floor((float)z / chunkSize);

        int localX = mod(x, chunkSize);
        int localZ = mod(z, chunkSize);

        Chunk *cptr = this->chunks[{cx, cz}];
            return &cptr->blocks[localX][y][localZ];
        
        //return nullptr;
    }
    Chunk* getChunkAtPos(glm::vec3 pos){
        int cx = floor((float)pos.x / chunkSize);
        int cz = floor((float)pos.z / chunkSize);


        return this->chunks[{cx, cz}];
    }
    void AddChunkUpdateQuaue(glm::vec3 pos){
        
        updatedChunks_indx.insert(getChunkAtPos(pos));

    }
    ~ChunkManager() = default;
};
