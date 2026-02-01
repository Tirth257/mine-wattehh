#pragma once
#include<bits/stdc++.h>
using namespace std;
const int chunkSize = 16;
const int chunkHeight = 64;

    int mod(int a, int b) {
    return ((a % b) + b) % b;
}