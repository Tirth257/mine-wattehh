#pragma once
#include <random>
#include <chrono>
#include <vector>
#include <algorithm>

class Random
{
public:
    static std::mt19937 gen; // Mersenne Twister generator

    // Initialize the generator (call once)
    static void init(unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count())
    {
        gen.seed(seed);
    }

    // Random integer in range [low, high]
    static int getInt(int low, int high)
    {
        std::uniform_int_distribution<int> dist(low, high);
        return dist(gen);
    }

    // Random double in range [low, high)
    static double getDouble(double low, double high)
    {
        std::uniform_real_distribution<double> dist(low, high);
        return dist(gen);
    }

    // Random double between [0, 1)
    static double getDouble()
    {
        return getDouble(0.0, 1.0);
    }
};

// Static member definition
std::mt19937 Random::gen(std::chrono::steady_clock::now().time_since_epoch().count());
