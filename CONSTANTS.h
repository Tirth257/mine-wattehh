#pragma once
#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"back/FastNoiseLite.h"
using namespace std;
const int chunkSize = 32;
const int chunkHeight = 64;
const int renderDistance = 20;
int mod(int a, int b)
{
    return ((a % b) + b) % b;
}
static const vector<glm::ivec3> flowDirections = {glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)};

    struct PairHash
    {
        size_t operator()(const std::pair<int, int> &p) const noexcept
        {
            size_t h1 = std::hash<int>{}(p.first);
            size_t h2 = std::hash<int>{}(p.second);
            return h1 ^ (h2 << 1);
        }
    };

    struct Vec3Hash {
    size_t operator()(const glm::ivec3& v) const {
        return ((std::hash<int>()(v.x) ^
               (std::hash<int>()(v.y) << 1)) >> 1) ^
               (std::hash<int>()(v.z) << 1);
    }
};

const int targetFPS = 60;
const chrono::duration<double> targetFrameDuration((double)1 / targetFPS);
float lightColor[3] = {1, 1, 1};
float lightPos[3] = {0, 300, 0};


FastNoiseLite *heightNoise;

