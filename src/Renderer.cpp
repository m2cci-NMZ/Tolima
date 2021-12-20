#include "../include/Renderer.h"
#include <iostream>
#include "Instrumentor.h"

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
    return EXIT_SUCCESS;
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
            case SDLK_a:
                camera.turn(0.02);
                break;
            case SDLK_z:
                camera.moveForward(1);
                break;
            case SDLK_s:
                camera.moveBackward(1);
                break;
            case SDLK_e:
                camera.turn(-0.02);
                break;
            case SDLK_d:
                camera.moveRight(1);
                break;
            case SDLK_q:
                camera.moveLeft(1);
                break;
            case SDLK_x:
                camera.moveDown(1);
                break;
            case SDLK_w:
                camera.moveUp(1);
                break;
            case SDLK_r:
                this->wireMode = !this->wireMode;
                break;
            case SDLK_t:
                this->wireMode = false;
                break;
            }

            break;
        }
    }
}

void Renderer::drawObject(TriMesh &object, const Point &campos, Shader &s)
{
    for (auto &tri : object.getTriangles())
    {
        if (this->wireMode)
        {
            this->drawWireTriangle(tri);
        }
        else
        {
            this->renderTriangle(tri, campos, s);
        }
    }
}
void Renderer::drawScene(Scene &scene, const Point &campos)
{
    for (int i = 0; i < scene.getNumObjects(); i++)
    {
        Object &&o = scene.getObject(i);
        Shader &&s = scene.getShaderById(o.getShaderId());
        this->drawObject(o, campos, s);
    }
}
int Renderer::closeWindow()
{
    SDL_DestroyRenderer(this->pRenderer);
    SDL_DestroyWindow(this->pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void Renderer::renderLoop(Camera &camera, Scene &scene, Clipper &clip)
{
    uint32_t startTime = SDL_GetTicks();
    double elapsedTime = 0;
    uint32_t counter = 0;

    while (this->isOpen)
    {
        {
            this->eventManager(camera);
        }

        if (counter > 10)
        {
            counter = 0;
            startTime = SDL_GetTicks();
        }

        this->resetzBuffer(10000000000.f);
        Scene &&transformedScene = this->transformScene(camera, scene, clip);

        SDL_SetRenderDrawColor(pRenderer, 255 / 2, 255 / 2, 255 / 2, 0);
        SDL_RenderClear(this->pRenderer);
        this->drawScene(transformedScene, camera.getPosition());

        SDL_RenderPresent(this->pRenderer);

        uint32_t currTime = SDL_GetTicks();
        elapsedTime = (currTime - startTime) / 1000.0;
        counter++;
        string title;

        title = to_string(double(counter) / elapsedTime);
        title.append(" FPS");
        const char *titleConverted = title.c_str();
        SDL_SetWindowTitle(pWindow, titleConverted);
    }
}
void Renderer::boundingBox(Triangle &t, float &xmin, float &xmax, float &ymin, float &ymax)
{
    // PROFILE_FUNCTION();
    float y[3] = {t.getA().getY(), t.getB().getY(), t.getC().getY()};
    float x[3] = {t.getA().getX(), t.getB().getX(), t.getC().getX()};

    xmin = *std::min_element(x, x + 3);
    ymin = *std::min_element(y, y + 3);
    xmax = *std::max_element(x, x + 3);
    ymax = *std::max_element(y, y + 3);
}
void Renderer::renderTriangle(Triangle &t, Point campos, Shader &s)
{
    PROFILE_FUNCTION();
    s.computeVertIntensities(t, campos);

    float xmin, xmax, ymin, ymax;
    this->boundingBox(t, xmin, xmax, ymin, ymax);

    float iAR, iBR, iCR;
    iAR = s.getIntensityAR();
    iBR = s.getIntensityBR();
    iCR = s.getIntensityCR();

    float iAG, iBG, iCG;
    iAG = s.getIntensityAG();
    iBG = s.getIntensityBG();
    iCG = s.getIntensityCG();

    float iAB, iBB, iCB;
    iAB = s.getIntensityAB();
    iBB = s.getIntensityBB();
    iCB = s.getIntensityCB();

    float a, b, c, d;
    float zx;
    t.derivePlane(a, b, c, d);
    float z = -(xmin * a + ymin * b + d) / c;

    int xmaxint = int(xmax);
    int ymaxint = int(ymax);
    int xminint = int(xmin);
    int yminint = int(ymin);

    Point p;

    p.setX(xminint);
    p.setY(yminint);
    float w1, w2, w3, area, w1x, w2x, w3x;
    w1 = edge(t.getB(), t.getC(), p);
    w2 = edge(t.getC(), t.getA(), p);
    w3 = edge(t.getA(), t.getB(), p);
    area = edge(t.getA(), t.getB(), t.getC());

    float x1, x2, x3;
    x1 = t.getA().getX();
    x2 = t.getB().getX();
    x3 = t.getC().getX();

    float y1, y2, y3;
    y1 = t.getA().getY();
    y2 = t.getB().getY();
    y3 = t.getC().getY();

    {
        PROFILE_SCOPE("drawing loop");
        for (int i = xminint; i <= xmaxint; i++)
        {
            // PROFILE_SCOPE("outer loop");
            zx = z;
            w1x = w1;
            w2x = w2;
            w3x = w3;
            for (int j = yminint; j <= ymaxint; j++)
            {
                // PROFILE_SCOPE("inner loop");
                if (w1x >= 0 && w2x >= 0 && w3x >= 0 && zx < zBuffer[i][j])
                {
                    // PROFILE_SCOPE("compute RGB");

                    float R = (w1x/area) * iAR + (w2x/area) * iBR + (w3x/area) * iCR;
                    float G = (w1x/area) * iAG + (w2x/area) * iBG + (w3x/area) * iCG;
                    float B = (w1x/area) * iAB + (w2x/area) * iBB + (w3x/area) * iCB;

                    SDL_SetRenderDrawColor(this->pRenderer, int(255 * R), int(255 * G), int(255 * B), 255);
                    SDL_RenderDrawPoint(this->pRenderer, i, j);
                    zBuffer[i][j] = zx;
                }
                zx = zx - a / c;
                w1x = w1x - (x3 - x2);
                w2x = w2x - (x1 - x3);
                w3x = w3x - (x2 - x1);
            }
            z = z - b / c;
            w1 = w1 + (y3 - y2);
            w2 = w2 + (y1 - y3);
            w3 = w3 + (y2 - y1);
        }
    }
}
Scene Renderer::transformScene(Camera &camera, Scene &scene, Clipper clip)
{
    PROFILE_FUNCTION();
    Point pNear, pNearNormal;
    pNear.setZ(0.1);
    pNearNormal.setZ(1.0);

    Point pLeft, pLeftNormal;
    pLeftNormal.setX(1.);

    Point pUp, pUpNormal;
    pUpNormal.setY(1.0);

    Point pRight, pRightNormal;
    pRight.setX(float(this->windowWidth));
    pRightNormal.setX(-1.0);

    Point pDown, pDownNormal;
    pDown.setY(this->windowHeight);
    pDownNormal.setY(-1.0);
    Scene s;
    for (int i = 0; i < scene.getNumObjects(); i++)
    {
        Object &&proj = scene.getObject(i);
        camera.worldTransform(proj);
        clip.setPlane(pNear, pNearNormal);
        clip.clipObject(proj);
        camera.ndcTransform(proj, this->windowHeight, this->windowWidth);

        camera.viewPortTransform(proj, this->windowHeight, this->windowWidth);

        clip.setPlane(pLeft, pLeftNormal);
        clip.clipObject(proj);

        clip.setPlane(pUp, pUpNormal);
        clip.clipObject(proj);

        clip.setPlane(pRight, pRightNormal);
        clip.clipObject(proj);

        clip.setPlane(pDown, pDownNormal);
        clip.clipObject(proj);
        Object transformedObject;
        transformedObject.setMesh(proj);
        transformedObject.setId(proj.getId());
        transformedObject.setShaderId(proj.getShaderId());
        s.addObject(transformedObject);
    }
    s.copyShaders(scene);
    return s;
}
float Renderer::edge(const Point &a, const Point &b, const Point &c)
{
    // PROFILE_FUNCTION();
    return (c.getX() - a.getX()) * (b.getY() - a.getY()) - (c.getY() - a.getY()) * (b.getX() - a.getX());
}
void Renderer::resetzBuffer(float depth)
{
    for (int i = 0; i < windowWidth; i++)
    {
        for (int j = 0; j < windowHeight; j++)
        {
            zBuffer[i][j] = depth;
        }
    }
}
void Renderer::drawWireTriangle(Triangle &t)
{
    SDL_SetRenderDrawColor(this->pRenderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(this->pRenderer, t.getA().getX(), t.getA().getY(), t.getB().getX(), t.getB().getY());
    SDL_RenderDrawLine(this->pRenderer, t.getA().getX(), t.getA().getY(), t.getC().getX(), t.getC().getY());
    SDL_RenderDrawLine(this->pRenderer, t.getB().getX(), t.getB().getY(), t.getC().getX(), t.getC().getY());
}