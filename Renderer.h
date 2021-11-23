#include <SDL2/SDL.h>
#include "Camera.h"
#include "TriMesh.h"
#include "Clipper.h"
#include "Shader.h"
#include "Scene.h"
#include <algorithm>
#include <cmath>
#include <vector>

class Renderer
{
private:
    int windowHeight;
    int windowWidth;
    SDL_Window *pWindow{nullptr};
    SDL_Renderer *pRenderer{nullptr};
    SDL_Event events;
    bool isOpen{true};

public:
    Renderer(int hieght, int width);
    int getWidth();
    int getHeight();
    void setWidth(int width);
    void setHeight(int height);
    Scene transformScene(Camera &camera, Scene &scene, Clipper clip);
    void drawScene(Scene scene, std::vector<std::vector<float>> &zbuffer, Point campos);
    void drawObject(TriMesh object, std::vector<std::vector<float>> &zbuffer, Point campos, Shader s);
    int init();
    void play();
    void eventManager(Camera &camera);
    void renderLoop(Camera camera, Scene scene, Clipper clip);
    int closeWindow();
    void boundingBox(Triangle &t, float &xmin, float &xmax, float &ymin, float &ymax);
    void renderTriangle(Triangle &t, std::vector<std::vector<float>> &zbuffer, Point campos, Shader s);
};
