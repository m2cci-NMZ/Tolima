#include <SDL2/SDL.h>
#include "Camera.h"
#include "TriMesh.h"
#include "Clipper.h"
#include "Shader.h"

class Renderer{
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
    void drawTriangle(Point p1, Point p2, Point p3);
    void drawObject(Trimesh object);
    int init();
    void play();
    void eventManager(Camera &camera);
    void renderLoop(Camera camera, TriMesh object);
    int closeWindow();
}
