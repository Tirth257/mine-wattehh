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

    ChunkManager(int squareSize)
    {
        this->squareSize=squareSize;
    }
    void genMapping()
    {
        for (int i = -squareSize / 2; i < squareSize / 2; i++)
        {
            for (int j = -squareSize / 2; j < squareSize / 2; j++)
            {
                Chunk *tempChunk = new Chunk({i,j});
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
    ~ChunkManager() = default;
};
