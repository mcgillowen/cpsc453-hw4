#include <iostream>
#include <fstream>

#include <math.h>

#include "RayTracer.h"
#include <fstream>
using std::ofstream;
using std::ifstream;

// store width and height of the render
int width = 512;
int height = 512;

// Our ray tracer
RayTracer * rt;

int main(int argc, char* argv[]){

    int depth = 8;
    int sampling = 4;

    if (argc > 2) {
        depth = atoi(argv[1]);
        sampling = atoi(argv[2]);
    }

    // Test scene with max depth of 4 and sampling of 1
    rt = new RayTracer(Scene::initTestScene(width),depth,sampling);
    if (argc == 4) rt = new RayTracer(Scene::initTestScene2(width),depth,sampling);
    float pixels[width][height][4];
    for(int ctr = 0; ctr < height*width; ctr++){
        int i = ctr % width;
        int j = ctr / width;
        Color rad = rt->calculate(i,j);
        //std::cout << i * j << " of " << height * width << std::endl;
        pixels[i][j][0] = rad.r; //Red
        pixels[i][j][1] = rad.g; //Green
        pixels[i][j][2] = rad.b; //Blue
        pixels[i][j][3] = 1.0; //Alpha
    }
    // once we are done calculating, we will write to file.
    ofstream testimage;
    testimage.open("TestScene.rgb",ios::binary | ios::out);
    for(int j = height-1; j >=0 ; j-- ) {
        for(int i = 0; i< width; i++){
            for(int k = 0; k < 3; k++){
                 // normalize color value to 0-255.
                 // This assumes that the color values are in the
                 // range [0,1].
                char c = int(pixels[i][j][k]*255);
                 testimage << c;
            }
        }
    }
    testimage.close();
    return 0;
}
