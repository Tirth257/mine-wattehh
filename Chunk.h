#pragma once
#include "Block.h"
#include <vector>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "CONSTANTS.h"
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
    // add the following Objects into a single array
    unsigned int VAO, VBO, VBO2;
    unsigned int water_VAO, water_VBO, water_VBO2;
    int vertexCount;
    vector<float> mesh;
    vector<int> typesdata;
    vector<float> water_mesh;
    vector<int> water_typesdata;

    bool isAir(int x, int y, int z)
    {
        if (x == -1 || y == -1 || z == -1 || x == chunkSize || y == chunkHeight || z == chunkSize)
        {
            return true;
        }

        return blocks[x][y][z].type == AIR;
    }
    bool isWater(int x, int y, int z)
    {
        if (x == -1 || y == -1 || z == -1 || x == chunkSize || y == chunkHeight || z == chunkSize)
        {
            return true;
        }

        return blocks[x][y][z].type == WATER;
    }
    void AddVertex(float x, float y, float z, float ox, float oy, float oz, vector<float> &mesh)
    {
        mesh.push_back(x + xoffset + ox);
        mesh.push_back(y + oy);
        mesh.push_back(z + zoffset + oz);
    }

    void AddFace(int face, int x, int y, int z)
    {
        if (blocks[x][y][z].type == WATER)
        {
            for (int i = 0; i < 6; i++)
            {
                AddVertex(x, y, z, face_vert[face][i][0], face_vert[face][i][1], face_vert[face][i][2], water_mesh);
                water_typesdata.push_back(blocks[x][y][z].waterLevel);
                // send normal vector index along with it typesdata for (Normal,).
                water_typesdata.push_back(face);
            }
        }
        else
        {

            for (int i = 0; i < 6; i++)
            {
                AddVertex(x, y, z, face_vert[face][i][0], face_vert[face][i][1], face_vert[face][i][2], mesh);
                typesdata.push_back(blocks[x][y][z].type);
                // send normal vector index along with it typesdata for (Normal,).
                typesdata.push_back(face);
            }
        }
    }
    float xoffset;
    float zoffset;

public:
    pair<int, int> chunk_coordinates;

    Block blocks[chunkSize][chunkHeight][chunkSize];

    Chunk(pair<int, int> chunk_coordinate)
    {
        this->chunk_coordinates = chunk_coordinate;
        this->xoffset = chunk_coordinate.first * chunkSize;
        this->zoffset = chunk_coordinate.second * chunkSize;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &VBO2);

        glGenVertexArrays(1, &water_VAO);
        glGenBuffers(1, &water_VBO);
        glGenBuffers(1, &water_VBO2);
    }
    // Generated Terrain
    void GenerateTerrain()
    {

        for (int x = 0; x < chunkSize; x++)
        {
            for (int z = 0; z < chunkSize; z++)
            {
                for (int y = 0; y < chunkHeight; y++)
                {
                    float val = (heightNoise->GetNoise(x + xoffset, z + zoffset) + 1) / 2;

                    if (y < val * chunkHeight)
                    {
                        blocks[x][y][z].type = DIRT;
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

        water_mesh.clear();
        water_typesdata.clear();

        for (int x = 0; x < chunkSize; x++)
        {
            for (int z = 0; z < chunkSize; z++)
            {
                for (int y = 0; y < chunkHeight; y++)
                {
                    BlockType block_type = blocks[x][y][z].type;
                    if (block_type == AIR)
                    {
                        continue;
                    }
                    if (block_type == WATER)
                    {
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
                    else
                    {

                        if (isAir(x + 1, y, z) || isWater(x + 1, y, z))
                            AddFace(0, x, y, z); // +X
                        if (isAir(x - 1, y, z) || isWater(x - 1, y, z))
                            AddFace(1, x, y, z); // -X
                        if (isAir(x, y + 1, z) || isWater(x, y + 1, z))
                            AddFace(2, x, y, z); // +Y
                        if (isAir(x, y - 1, z) || isWater(x, y - 1, z))
                            AddFace(3, x, y, z); // -Y
                        if (isAir(x, y, z + 1) || isWater(x, y, z + 1))
                            AddFace(4, x, y, z); // +Z
                        if (isAir(x, y, z - 1) || isWater(x, y, z - 1))
                            AddFace(5, x, y, z); // -Z
                    }
                }
            }
        }
    }
    void AppendBlock(int x, int y, int z)
    {
        BlockType block_type = blocks[x][y][z].type;
        if (block_type == AIR)
        {
            return;
        }
        if (block_type == WATER)
        {
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
        else
        {

            if (isAir(x + 1, y, z) || isWater(x + 1, y, z))
                AddFace(0, x, y, z); // +X
            if (isAir(x - 1, y, z) || isWater(x - 1, y, z))
                AddFace(1, x, y, z); // -X
            if (isAir(x, y + 1, z) || isWater(x, y + 1, z))
                AddFace(2, x, y, z); // +Y
            if (isAir(x, y - 1, z) || isWater(x, y - 1, z))
                AddFace(3, x, y, z); // -Y
            if (isAir(x, y, z + 1) || isWater(x, y, z + 1))
                AddFace(4, x, y, z); // +Z
            if (isAir(x, y, z - 1) || isWater(x, y, z - 1))
                AddFace(5, x, y, z); // -Z
        }
    }
    void uploadToGPU()
    {
        // for solid mesh
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.size() * sizeof(float), mesh.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, VBO2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * typesdata.size(), typesdata.data(), GL_STATIC_DRAW);
        glVertexAttribIPointer(1, 1, GL_INT, 2 * sizeof(int), 0);
        glVertexAttribIPointer(2, 1, GL_INT, 2 * sizeof(int), (void *)(sizeof(int)));
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // for water mesh

        glBindVertexArray(water_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, water_VBO);

        glBufferData(GL_ARRAY_BUFFER, water_mesh.size() * sizeof(float), water_mesh.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, water_VBO2);

        glBufferData(GL_ARRAY_BUFFER, sizeof(int) * water_typesdata.size(), water_typesdata.data(), GL_STATIC_DRAW);
        glVertexAttribIPointer(1, 1, GL_INT, 2 * sizeof(int), 0);
        glVertexAttribIPointer(2, 1, GL_INT, 2 * sizeof(int), (void *)(sizeof(int)));
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }

    void draw(unsigned int *program, unsigned int *water_program) // is called in ChunkManager Only
    {

        glBindVertexArray(VAO);
        glUseProgram(*program);
        glDrawArrays(GL_TRIANGLES, 0, mesh.size() / 3); // Change Last Argument when adding U V etc..

        if (!water_mesh.empty())
        {
            glUseProgram(*water_program);
            glBindVertexArray(water_VAO);
            // glPatchParameteri(GL_PATCH_VERTICES, 3);
            glDrawArrays(GL_TRIANGLES, 0, water_mesh.size() / 3);
        }

        glBindVertexArray(0);
    }

    // Chunk menuplation
    ~Chunk()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &VBO2);

        glDeleteVertexArrays(1, &water_VAO);
        glDeleteBuffers(1, &water_VBO);
        glDeleteBuffers(1, &water_VBO2);

        cout << "Freed chunk at " << chunk_coordinates.first << " " << chunk_coordinates.second << endl;
    }
};
