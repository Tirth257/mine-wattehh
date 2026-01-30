#pragma once
#include "Block.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;
static const float face_vert[6][6][3] = {
    // +X (right)
    {{0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}},
    // -X (left)
    {{-0.5, -0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}},
    // +Y (top)
    {{-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}},
    // -Y (bottom)
    {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}},
    // +Z (front)
    {{-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}},
    // -Z (back)
    {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}}};

class Chunk
{
private:
    unsigned int VAO, VBO;
    int vertexCount;
    vector<float> mesh;
    vector<int> typesdata;
    bool isAir(int x, int y, int z)
    {
        if (x == -1 || y == -1 || z == -1 || x == SIZE || y == SIZE || z == SIZE)
        {
            return true;
        }

        return (blocks[x][y][z].type == AIR);
    }
    void AddVerex(float x, float y, float z, float ox, float oy, float oz)
    {
        mesh.push_back(x + ox);
        mesh.push_back(y + oy);
        mesh.push_back(z + oz);
    }

    void AddFace(int face, int x, int y, int z)
    {
        for (int i = 0; i < 6; i++)
        {
            AddVerex(x, y, z, face_vert[face][i][0], face_vert[face][i][1], face_vert[face][i][2]);
            typesdata.push_back(blocks[x][y][z].type);
        }
    }

public:
    static const int SIZE = 16;

    Block blocks[SIZE][SIZE][SIZE];

    // Generated Terrain
    void GenerateTerrain()
    {
        for (int x = 0; x < SIZE; x++)
        {
            for (int z = 0; z < SIZE; z++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    blocks[x][y][z].type = BlockType::STONE;
                    blocks[x][y][z].type = BlockType::DIRT;
                }
            }
        }
    }
    void buildMesh()
    {
        mesh.clear();
        for (int x = 0; x < SIZE; x++)
        {
            for (int z = 0; z < SIZE; z++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    if (isAir(x, y, z))
                    {
                        continue;
                    }
                    if (isAir(x + 1, y, z))
                        AddFace(0, x, y, z);
                    if (isAir(x - 1, y, z))
                        AddFace(1, x, y, z); // -X
                    if (isAir(x, y + 1, z))
                        AddFace(2, x, y, z); // +Y
                    if (isAir(x, y - 1, z))
                        AddFace(3, x, y, z); // -Y
                    if (isAir(x, y, z + 1))
                        AddFace(4, x, y, z); // +Z
                    if (isAir(x, y, z - 1))
                        AddFace(5, x, y, z); // -Z
                }
            }
        }
    }
    void uploadToGPU()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        unsigned int VBO2;
        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * typesdata.size(), typesdata.data(), GL_STATIC_DRAW);
        glVertexAttribIPointer(1, 1, GL_INT, 0, 0);

        glEnableVertexAttribArray(1);
    }

    // will be called outside
    void draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 3); // Change Last Argument when adding U V etc..
    }

    // Chunk menuplation
    void destroyBlock(int x, int y, int z, bool build, bool upload)
    {

        blocks[x][y][z].type = AIR;

        if (build)
        {
            buildMesh();
        }

        if (upload)
        {
            uploadToGPU();
        }
    }
    void destroyBlock(Block *block, bool build, bool upload)
    {

        block->type = AIR;

        if (build)
        {
            buildMesh();
        }

        if (upload)
        {
            uploadToGPU();
        }
    }
};
