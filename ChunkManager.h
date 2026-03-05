#pragma once
#include "Chunk.h"
#include "CONSTANTS.h"
class ChunkManager
{
private:
    int distance(int i, int j)
    {
        return abs(i - cam->cameraPos.x) + abs(j - cam->cameraPos.z);
    }
    int distance(pair<int, int> coord)
    {
        return abs(coord.first - cam->cameraPos.x) + abs(coord.second - cam->cameraPos.z);
    }
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
    unsigned int *program;
    unsigned int *water_program;

    FastNoiseLite heightMap;

public:
    std::unordered_map<pair<int, int>, Chunk *, PairHash> chunks;
    std::unordered_set<Chunk *> updatedChunks_indx;

    std::unordered_map<pair<int, int>, Chunk *, PairHash> to_draw_chunk;

    ChunkManager( unsigned int *program, unsigned int *water_program)
    {
        this->program = program;
        this->water_program = water_program;
    }
    void genMapping()
    {
        int x = 0;
        int z = 0;
        for (int i = -renderDistance / 2; i <= renderDistance / 2; i++)
        {
            for (int j = -renderDistance / 2 + abs(i); j <= renderDistance / 2 - abs(i); j++)
            {

                
                if (chunks.find({x + i, z + j}) != chunks.end()) // already generated
                {
                    to_draw_chunk[{i+x, j+z}] = chunks[{i+x, j+z}];
                }
                else
                {

                    Chunk *tempChunk = new Chunk({i+x, j+z});
                    tempChunk->GenerateTerrain();
                    tempChunk->buildMesh();
                    tempChunk->uploadToGPU();
                    chunks[{i+x, j+z}] = tempChunk;
                    to_draw_chunk[{i+x, j+z}] = tempChunk;
                }
            }
        }
    }

    void updateMapping2() // TODO: Finish this efficient algorith
    {
        // remove far chunks
        for (auto &&chk : to_draw_chunk)
        {
            if (distance(chk.first) > renderDistance / 2)
            {
                to_draw_chunk.erase(chk.first);
            }
        }

        // add closer chunks
        pair<int, int> chunk_chunk_coordinate = getChunkAtPos(cam->cameraPos)->chunk_coordinates;
    }

    void updateMapping()
    {
        to_draw_chunk.clear();
        
        // add closer chunks
        pair<int, int> chunk_coordinate = getChunkAtPos(cam->cameraPos)->chunk_coordinates;
        int x = chunk_coordinate.first;
        int z = chunk_coordinate.second;

/*
        for (int i = x-renderDistance; i <= x + renderDistance; i++)
        {
            for (int j = z-renderDistance+abs(i-renderDistance); j <= z+renderDistance-abs(i-renderDistance); j++)
            {
               if (chunks.find({i, j}) != chunks.end()) // already generated
                {
                    to_draw_chunk[{i, j}] = chunks[{i, j}];
                }
                else
                {

                    Chunk *tempChunk = new Chunk({i, j});
                    tempChunk->GenerateTerrain();
                    tempChunk->buildMesh();
                    tempChunk->uploadToGPU();
                    chunks[{i, j}] = tempChunk;
                    to_draw_chunk[{i, j}] = tempChunk;
                    cout << " creating a lot "<< endl  ;
                }  
            }
            
        }
        
*/

        for (int i = -renderDistance / 2; i <= renderDistance / 2; i++)
        {
            for (int j = -renderDistance / 2 + abs(i); j <= renderDistance / 2 - abs(i); j++)
            {

                
                if (chunks.find({x + i, z + j}) != chunks.end()) // already generated
                {
                    to_draw_chunk[{i+x, j+z}] = chunks[{i+x, j+z}];
                }
                else
                {

                    Chunk *tempChunk = new Chunk({i+x, j+z});
                    tempChunk->GenerateTerrain();
                    tempChunk->buildMesh();
                    tempChunk->uploadToGPU();
                    chunks[{i+x, j+z}] = tempChunk;
                    to_draw_chunk[{i+x, j+z}] = tempChunk;
                }
            }
        }
    }

    void draw()
    {

        for (auto &&i : to_draw_chunk)
        {
            i.second->draw(program, water_program);
        }

    }

    void updateBatchChunk()
    {

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

        // return nullptr;
    }
    Chunk *getChunkAtPos(glm::vec3 pos)
    {
        int cx = floor((float)pos.x / chunkSize);
        int cz = floor((float)pos.z / chunkSize);

        return this->chunks[{cx, cz}];
    }
    void AddChunkUpdateQuaue(glm::vec3 pos)
    {

        updatedChunks_indx.insert(getChunkAtPos(pos));
    }
    void setReferenceCamera(Camera *cam)
    {
        this->cam = cam;
    }
    ~ChunkManager() = default;
};
