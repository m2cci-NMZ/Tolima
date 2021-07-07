#include "Renderer.h"

Renderer::Renderer(int height, int width)
{
    this->windowHeight = height;
    this->windowWidth = width;
}

int Renderer::getWidth()
{
    return this->windowWidth;
}
int Renderer::getHeight()
{
    return this->windowHeight;
}

int Renderer::init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    if (SDL_CreateWindowAndRenderer(this->windowWidth, this->windowHeight, SDL_WINDOW_SHOWN, &this->pWindow, &this->pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
}

void Renderer::eventManager(Camera &camera)
{
    while (SDL_PollEvent(&this->events))
    {
        switch (this->events.type)
        {
        case SDL_QUIT:
            this->isOpen = false;
            break;
        case SDL_KEYDOWN:
            switch (this->events.key.keysym.sym)
            {
            case SDLK_q:
                fYaw += elapsedTime * 0.02;
                camera.turn(fYaw);
                break;
            case SDLK_z:
                camera.moveForward(1);
                break;
            case SDLK_s:
                camera.moveBackward(1);
                break;
            case SDLK_d:
                fYaw -= elapsedTime * 0.02;
                camera.turn(fYaw);
                break;
            case SDLK_x:
                camera.moveDown(1);
                break;
            case SDLK_w:
                camera.moveUp(1);
                break;
            }

            break;
        }
    }
}

void Renderer::drawTriangle(Point p1, Point p2, Point p3)
{
    //@todo this should ideally be somwhere else, and just return some sort of list with the coordinates of the pixels that should be colored.
    //get length of all sides
    float d1, d2, d3, tx, ty, vx, vy;
    int counter;

    d1 = sqrt(((p2.getY() - p1.getY()) * (p2.getY() - p1.getY())) + ((p2.getX() - p1.getX()) * (p2.getX() - p1.getX())));
    d2 = sqrt(((p3.getY() - p2.getY()) * (p3.getY() - p2.getY())) + ((p3.getX() - p2.getX()) * (p3.getX() - p2.getX())));
    d3 = sqrt(((p1.getY() - p3.getY()) * (p1.getY() - p3.getY())) + ((p1.getX() - p3.getX()) * (p1.getX() - p3.getX())));
    if (((d1 < d2) or (d1 = d2)) and ((d1 < d2) or (d1 = d2)))
    { //the first side is the shortest
        tx = p1.getX();
        ty = p1.getY();
        vx = (p2.getX() - p1.getX()) / d1;
        vy = (p2.getY() - p1.getY()) / d1;
        counter = 0;
        while (counter < d1)
        {
            SDL_RenderDrawLine(this->pRenderer, p3.getX(), p3.getY(), tx, ty);
            //drawing a line from point(p3.getX(),p3.getY()) to point(tx,ty).
            tx = tx + vx;
            ty = ty + vy;
            counter = counter + 1;
        }
    }
    else if ((d2 < d3) or (d2 = d3))
    { //the second side is the shortest
        tx = p2.getX();
        ty = p2.getY();
        vx = (p3.getX() - p2.getX()) / d2;
        vy = (p3.getY() - p2.getY()) / d2;
        counter = 0;

        while (counter < d2)
        {
            SDL_RenderDrawLine(this->pRenderer, p1.getX(), p1.getY(), tx, ty);
            tx = tx + vx;
            ty = ty + vy;
            counter = counter + 1;
        }
    }
    else
    { // the third side is shortest
        tx = p3.getX();
        ty = p3.getY();
        vx = (p1.getX() - p3.getX()) / d3;
        vy = (p1.getY() - p3.getY()) / d3;
        counter = 0;

        while (counter < d3)
        {
            SDL_RenderDrawLine(this->pRenderer, p2.getX(), p2.getY(), tx, ty);
            tx = tx + vx;
            ty = ty + vy;
            counter = counter + 1;
        }
    }
}
void Renderer::drawObject(TriMesh object)
{
    for (auto tri::object.getTriangles())
    {
        this->drawTriangle(tri.getA(), tri.getB(), tri.getC());
    }
}
int Renderer::closeWindow()
{
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void Renderer::renderLoop(Camera camera, TriMesh object, Shader shader, Clipper clip)
{
    //this should be done directly by the Clipper object
    Point pNear, pNearNormal;
    pNear.setZ(0.1);
    pNearNormal.setZ(1.0);

    Point pLeft, pLeftNormal;
    pLeftNormal.setX(1.);

    Point pUp, pUpNormal;
    pUpNormal.setY(1.0);

    Point pRight, pRightNormal;
    pRight.setX(float(width));
    pRightNormal.setX(-1.0);

    Point pDown, pDownNormal;
    pDown.setY(height);
    pDownNormal.setY(-1.0);
    while (this->isOpen)
    {
        {
            this->eventManager(&camera);
        }
        TriMesh proj;

        proj = shader.computeShader(cube);
        proj = camera.worldTransform(proj);
        clip.setPlane(pNear, pNearNormal);
        proj = clip.clipObject(proj);
        proj = camera.ndcTransform(proj, this->windowHeight, this->windowWidth);

        proj = camera.viewPortTransform(proj, this->windowHeight, this->windowWidth);

        clip.setPlane(pLeft, pLeftNormal);
        proj = clip.clipObject(cubeProj);

        clip.setPlane(pUp, pUpNormal);
        proj = clip.clipObject(proj);

        clip.setPlane(pRight, pRightNormal);
        proj = clip.clipObject(proj);

        clip.setPlane(pDown, pDownNormal);
        proj = clip.clipObject(proj);
        this->drawObject(proj);
    }
}