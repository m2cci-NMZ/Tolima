#include <SDL2/SDL.h>
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>

#include "../include/Matrix.h"
#include "../include/TriMesh.h"
#include "../include/Point.h"
#include "../include/Triangle.h"
#include "../include/Loader.h"
#include "../include/Camera.h"
#include "../include/Clipper.h"
#include "../include/Shader.h"
#include "../include/Renderer.h"
#include "../include/Instrumentor.h"

using namespace std;


int main(int argc, char *argv[])
{
    Renderer renderer(480,720);
    //Renderer renderer(50,50);
    renderer.init();

    Camera camera;

    Shader shader;

    Clipper clip;

    Scene scene;
    
    string f = argv[1];
    string fshader = argv[2];
    Instrumentor::Get().BeginSession("Session Name");  
    Loader loader(f,fshader, &scene);
    if (!loader.loadMeshFromFile())
    {
        return 0;
    }

    renderer.renderLoop(camera, scene, clip);
    Instrumentor::Get().EndSession();
    return renderer.closeWindow();
}
