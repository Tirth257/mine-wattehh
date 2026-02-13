#pragma once
#include <bits/stdc++.h>
using namespace std;
const int chunkSize = 16;
const int chunkHeight = 64;

int mod(int a, int b)
{
    return ((a % b) + b) % b;
}
const int targetFPS = 60;
const chrono::duration<double> targetFrameDuration((double)1 / targetFPS);
float lightColor[3] = {1, 1, 1};
float lightPos[3] = {1, 20, 1};
