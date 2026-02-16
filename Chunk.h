#pragma once
#include "Block.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "CONSTANTS.h"
#include "back/Noises.h"
using namespace std;
static const float face_vert[6][6][3] = {
    // +X (right)
    {{0.5, 0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, -0.5, -0.5}},
    // -X (left)
    {{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}},
    // +Y (top)
    {{-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5}},
    // -Y (bottom)
    {{-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {-0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {-0.5, -0.5, 0.5}},
    // +Z (front)
    {{-0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}},
    // -Z (back)
    {{-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, -0.5, -0.5}}};

class Chunk
{
private:
    unsigned int VAO, VBO, VBO2;
    int vertexCount;
    vector<float> mesh;
    vector<int> typesdata;
    vector<float> water_mesh;
    bool isAir(int x, int y, int z)
    {
        if (x == -1 || y == -1 || z == -1 || x == chunkSize || y == chunkHeight || z == chunkSize)
        {
            return true;
        }

        return (blocks[x][y][z].type == AIR);
    }
    void AddVertex(float x, float y, float z, float ox, float oy, float oz)
    {
        mesh.push_back(x + xoffset + ox);
        mesh.push_back(y + oy);
        mesh.push_back(z + zoffset + oz);
        
    }

    void AddFace(int face, int x, int y, int z)
    {
        for (int i = 0; i < 6; i++)
        {
            AddVertex(x, y, z, face_vert[face][i][0], face_vert[face][i][1], face_vert[face][i][2]);
            typesdata.push_back(blocks[x][y][z].type);
            // send normal vector index along with it typesdata for (Normal,).
            typesdata.push_back(face);
        }
    }
    pair<int, int> chunk_coordinates;
    int xoffset;
    int zoffset;

public:
    Block blocks[chunkSize][chunkHeight][chunkSize];

    Chunk(pair<int, int> chunk_coordinate)
    {
        this->chunk_coordinates = chunk_coordinate;
        this->xoffset = chunk_coordinate.first * chunkSize;
        this->zoffset = chunk_coordinate.second * chunkSize;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &VBO2);
    }
    // Generated Terrain
    void GenerateTerrain()
    {
        vector<vector<color>> HeightMap = HeightMap::GenPerlineNoice(5,chunkSize,chunkSize);
        for (int x = 0; x < chunkSize; x++)
        {
            for (int z = 0; z < chunkSize; z++)
            {
                for (int y = 0; y < chunkHeight; y++)
                {
                    if (y < HeightMap[x][z][0]*chunkHeight)
                    {
                        blocks[x][y][z].type = DIRT;
                    }
                    else if (y < HeightMap[x][z][0]*chunkHeight/5)
                    {
                        blocks[x][y][z].type = GRASS;
                    }
                    
                    else
                    {
                        blocks[x][y][z].type = AIR;
                    }
                }
            }
        }
    }
    void buildMesh()
    {
        mesh.clear();
        typesdata.clear();
        for (int x = 0; x < chunkSize; x++)
        {
            for (int z = 0; z < chunkSize; z++)
            {
                for (int y = 0; y < chunkHeight; y++)
                {
                    if (isAir(x, y, z))
                    {
                        continue;
                    }/*
                    if (blocks[x][y][z].type == WATER)
                    {
                        // currently do it here , but leter change it
                        for (int i = 0; i < 6; i++)
                        {
                            AddVertex(x, y, z, face_vert[i][0][0], face_vert[i][0][1], face_vert[i][0][2]);
                            typesdata.push_back(blocks[x][y][z].type);
                        }
                        continue;
                    }*/
                    
                    if (isAir(x + 1, y, z)) 
                        AddFace(0, x, y, z); // +X
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
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * typesdata.size(), typesdata.data(), GL_STATIC_DRAW);
        glVertexAttribIPointer(1, 1, GL_INT, 2*sizeof(int), 0);
        glVertexAttribIPointer(2,1,GL_INT,2*sizeof(int),(void*)(sizeof(int)));
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);


    }

    void draw() // is called in ChunkManager Only 
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 3); // Change Last Argument when adding U V etc..
        glBindVertexArray(0);


    }

    // Chunk menuplation
};
