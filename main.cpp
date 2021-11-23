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

    Camera camera;

    Shader shader;

    Clipper clip;

    Scene scene;
    
    string f = argv[1];

    Loader loader(f, &scene);
    if (!loader.loadMeshFromFile())
    {
        return 0;
    }

    renderer.renderLoop(camera, scene, clip);

    return renderer.closeWindow();
}
