#pragma once
#include <bits/stdc++.h>
#include "Random.h"
#include "Vector.h"
using namespace std;
class Helper
{
public:
    // transits from a to b as x goes from 0 to 1
    static double lerp(double a, double b, double x)
    {
        return a + (b - a) * x;
    };
};
class HeightMap
{
private:
    static double Interplation(double t)
    {

        return t * t * t * (10 + t * (-15 + 6 * t));
    }

public:
    static void GenHeightMap(vector<vector<color>> &grid, int height, int width)
    {
        for (int H = 0; H < height; H++)
        {
            for (int W = 0; W < width; W++)
            {
                double val = Random::getDouble();
                for (int i = 0; i < 3; i++)
                {
                    grid[H][W].e[i] = val;
                }
            }
        }
    }
    static vector<vector<color>> GenHeightMap(int height, int width)
    {
        vector<vector<color>> grid(height, vector<color>(width));

        for (int H = 0; H < height; H++)
        {
            for (int W = 0; W < width; W++)
            {

                grid[H][W].e[0] = Random::getDouble();
                grid[H][W].e[1] = Random::getDouble();
                grid[H][W].e[2] = Random::getDouble();
            }
        }
        return grid;
    }
    static vector<vector<color>> GenPerlineNoice(int boxDim, int height, int width)
    {
        vector<vector<color>> grid(height, vector<color>(width));

        vector<vector<vec3>> vex(height / boxDim + 1, vector<vec3>(width / boxDim + 1));

        for (auto &&row : vex)
        {
            for (auto &&colmn : row)
            {
                colmn = random_unit_vector();
            }
        }

        for (int r = 0; r < height; r++)
        {
            for (int c = 0; c < width; c++)
            {

                int localR = r % boxDim;
                int localC = c % boxDim;

                double x = (double)localC / boxDim;
                double y = (double)localR / boxDim;

                vec3 d00(x, y, 0);
                vec3 d10(x - 1, y, 0);
                vec3 d01(x, y - 1, 0);
                vec3 d11(x - 1, y - 1, 0);

                double p0 = dot(d00, vex[r / boxDim][c / boxDim]);
                double p1 = dot(d10, vex[r / boxDim][c / boxDim + 1]);
                double p2 = dot(d01, vex[r / boxDim + 1][c / boxDim]);
                double p3 = dot(d11, vex[r / boxDim + 1][c / boxDim + 1]);

                double u = Interplation(x);
                double v = Interplation(y);

                double nx0 = Helper::lerp(p0, p1, u);
                double nx1 = Helper::lerp(p2, p3, u);
                double final = Helper::lerp(nx0, nx1, v);

                final = final * 0.5 + 0.5;
                final = std::clamp(final, 0.0, 1.0);
            
                grid[r][c].e[0] = final;
                grid[r][c].e[1] = final;
                grid[r][c].e[2] = final;
            }
        }
        return grid;
    }
};
