#pragma once
#include "Vector.h"
#include<vector>
#include <fstream>
using namespace std;
class Visualize
{
public:
    static void Save2DImage(vector<vector<color>> &grid, string name)
    {
        std::ofstream file_out(name, ios::out);
        int height = grid.size();
        int width = grid.back().size();
        file_out << "P3" << endl;
        file_out << width << " " << height << endl << 255 << endl;
        
        for (auto &&row : grid)
        {
            for (auto &&eachpiXel : row)
            {
                vector<int> writableColors = eachpiXel.getWritableColor();
                file_out << writableColors[0] << " " << writableColors[1] << " " << writableColors[2] << " ";
            }
            file_out << endl;
        }
        cout << "Sucssessfully written in ppm" << endl;
    };
};