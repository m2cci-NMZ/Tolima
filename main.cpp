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
    Renderer renderer(400,400);
    renderer.init()


    TriMesh cube;

    Camera camera;

    Shader shader;


    string f = "teapot.obj";
    Loader loader(f, &cube);
    if (!loader.loadMeshFromFile())
    {
        cube.readfile("exemple.txt");
    }




    Clipper clip;
    render.renderLoop(camera, cube, shader, clipper);

    return renderer.closeWindow();
}
