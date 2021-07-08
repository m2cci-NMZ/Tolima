#include <SDL2/SDL.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

#include "Matrix.h"
#include "TriMesh.h"
#include "Point.h"
#include "Triangle.h"
#include "Loader.h"
#include "Camera.h"
#include "Clipper.h"
#include "Shader.h"
#include "Renderer.h"

using namespace std;


int main(int argc, char *argv[])
{
    Renderer renderer(480,720);
    
    renderer.init();


    TriMesh cube;

    Camera camera;

    Shader shader;

    Clipper clip;


    string f = argv[1];

    Loader loader(f, &cube);
    if (!loader.loadMeshFromFile())
    {
        cube.readfile("exemple.txt");
    }

    renderer.renderLoop(camera, cube, shader, clip);

    return renderer.closeWindow();
}
