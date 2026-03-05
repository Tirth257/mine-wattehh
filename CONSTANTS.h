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
static const vector<glm::vec3> flowDirections = {glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 0, -1)};

const int targetFPS = 60;
const chrono::duration<double> targetFrameDuration((double)1 / targetFPS);
float lightColor[3] = {1, 1, 1};
float lightPos[3] = {0, 300, 0};


FastNoiseLite *heightNoise;

