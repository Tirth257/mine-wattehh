#pragma once
#include "Block.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
using namespace std;
class Chunk
{
private:
    unsigned int VAO, VBO;
    int vertexCount;
    vector<float> mesh;

    bool isAir(int x, int y, int z)
    {
        if (x == -1 || y == -1 || z == -1 || x == SIZE || y == SIZE || z == SIZE)
        {
            return false;
        }

        return (blocks[x][y][z].type == AIR);
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
                    if (y < SIZE / 2)
                    {
                        blocks[x][y][z].type = BlockType::STONE;
                    }
                    else
                    {
                        blocks[x][y][z].type = BlockType::AIR;
                    }
                }
            }
        }
    }
    void buildMesh()
    {
        for (int x = 0; x < SIZE; x++)
        {
            for (int z = 0; z < SIZE; z++)
            {
                for (int y = 0; y < SIZE; y++)
                {
                    for (int inX = -1; inX <= 1; inX += 2)
                    {
                        if (isAir(x + inX, y, z))
                        {
                            vector<float> p1 = {x + inX * 0.5f, y + inX * 0.5f, z + inX * 0.5f};
                            vector<float> p2 = {x + inX * 0.5f, y - inX * 0.5f, z + inX * 0.5f};
                            vector<float> p3 = {x + inX * 0.5f, y - inX * 0.5f, z - inX * 0.5f};
                            vector<float> p4 = {x + inX * 0.5f, y + inX * 0.5f, z - inX * 0.5f};

                            mesh.insert(mesh.end(), p1.begin(), p1.end());
                            mesh.insert(mesh.end(), p2.begin(), p2.end());
                            mesh.insert(mesh.end(), p3.begin(), p3.end());
                            mesh.insert(mesh.end(), p4.begin(), p4.end());
                        }
                    }
                    for (int inX = -1; inX <= 1; inX += 2)
                    {
                        if (isAir(x, y + inX, z))
                        {
                            vector<float> p1 = {x + inX * 0.5f, y + inX * 0.5f, z + inX * 0.5f};
                            vector<float> p2 = {x - inX * 0.5f, y + inX * 0.5f, z + inX * 0.5f};
                            vector<float> p3 = {x - inX * 0.5f, y + inX * 0.5f, z - inX * 0.5f};
                            vector<float> p4 = {x + inX * 0.5f, y + inX * 0.5f, z - inX * 0.5f};

                            mesh.insert(mesh.end(), p1.begin(), p1.end());
                            mesh.insert(mesh.end(), p2.begin(), p2.end());
                            mesh.insert(mesh.end(), p3.begin(), p3.end());
                            mesh.insert(mesh.end(), p4.begin(), p4.end());
                        }
                    }
                    for (int inX = -1; inX <= 1; inX += 2)
                    {
                        if (isAir(x, y, z + inX))
                        {
                            vector<float> p1 = {x + inX * 0.5f, y + inX * 0.5f, z + inX * 0.5f};
                            vector<float> p2 = {x + inX * 0.5f, y - inX * 0.5f, z + inX * 0.5f};
                            vector<float> p3 = {x - inX * 0.5f, y - inX * 0.5f, z + inX * 0.5f};
                            vector<float> p4 = {x - inX * 0.5f, y + inX * 0.5f, z + inX * 0.5f};

                            mesh.insert(mesh.end(), p1.begin(), p1.end());
                            mesh.insert(mesh.end(), p2.begin(), p2.end());
                            mesh.insert(mesh.end(), p3.begin(), p3.end());
                            mesh.insert(mesh.end(), p4.begin(), p4.end());
                        }
                    }
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
        glVertexAttribPointer(0, sizeof(float), GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(0);
    }
    void draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    }
};
