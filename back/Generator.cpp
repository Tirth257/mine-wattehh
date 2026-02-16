#include"bits/stdc++.h"
#include"Vector.h"
#include"ShowR.h"
#include"Noises.h"
using namespace std;
int main(int argc, char const *argv[])
{
    int height = 500;
    int width = 500;
    //vector<vector<color>> grid(height,vector<color>(width,color(0,0,0)));
    //HeightMap::GenHeightMap(grid,height,width);
    vector<vector<color>> perl = HeightMap::GenHeightMap(height,width);

    //Visualize::Save2DImage(grid,"ahah.ppm");
    Visualize::Save2DImage(perl,"perlin.ppm");
    
    return 0;
}
