#include <SDL2/SDL.h>

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
    int init();
    void play();
    void eventManager(Camera &camera);
}