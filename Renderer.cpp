#include "Renderer.h"
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
            }

            break;
        }
    }
}

void Renderer::drawObject(TriMesh object, std::vector<std::vector<float>> &zbuffer, Point campos, Shader s)
{
    PROFILE_FUNCTION();
    for (auto tri : object.getTriangles())
    {
        // SDL_SetRenderDrawColor(this->pRenderer, int(255 * tri.getLum()), int(255 * tri.getLum()), int(255 * tri.getLum()), 255);
        // this->drawTriangle(tri.getA(), tri.getB(), tri.getC());
        this->renderTriangle(tri, zbuffer, campos, s);
    }
}
void Renderer::drawScene(Scene scene, std::vector<std::vector<float>> &zbuffer, Point campos)
{
    for (int i = 0; i < scene.getNumObjects(); i++)
    {
        Object o = scene.getObject(i);
        Shader s = scene.getShaderById(o.getShaderId());
        this->drawObject(o, zbuffer, campos, s);
    }
}
int Renderer::closeWindow()
{
    SDL_DestroyRenderer(this->pRenderer);
    SDL_DestroyWindow(this->pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}

void Renderer::renderLoop(Camera camera, Scene scene, Clipper clip)
{
    // this should be done directly by the Clipper object
    /*Point pNear, pNearNormal;
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
*/
    uint32_t startTime = SDL_GetTicks();
    double elapsedTime = 0;
    uint32_t counter = 0;

    while (this->isOpen)
    {
        {
            this->eventManager(camera);
        }

        if (counter > 100)
        {
            counter = 0;
            startTime = SDL_GetTicks();
        }

        // TriMesh proj;

        std::vector<std::vector<float>> zbuffer(this->windowHeight + 1, std::vector<float>(this->windowWidth + 1, 100.0));
        PROFILE_FUNCTION();
        //        Object object = scene.getObject(0);
        //        TriMesh &&proj = camera.worldTransform(object);
        Scene &&transformedScene = this->transformScene(camera, scene, clip);
        /*
                clip.setPlane(pNear, pNearNormal);
                clip.clipObject(proj);
                proj = camera.ndcTransform(proj, this->windowHeight, this->windowWidth);

                proj = camera.viewPortTransform(proj, this->windowHeight, this->windowWidth);

                clip.setPlane(pLeft, pLeftNormal);
                clip.clipObject(proj);

                clip.setPlane(pUp, pUpNormal);
                clip.clipObject(proj);

                clip.setPlane(pRight, pRightNormal);
                clip.clipObject(proj);

                clip.setPlane(pDown, pDownNormal);
                clip.clipObject(proj);
        */
        SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 0);
        SDL_RenderClear(this->pRenderer);
        //this->drawObject(proj, zbuffer, camera.getPosition());
        this->drawScene(transformedScene,zbuffer,camera.getPosition());

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
    float y[3] = {t.getA().getY(), t.getB().getY(), t.getC().getY()};
    float x[3] = {t.getA().getX(), t.getB().getX(), t.getC().getX()};

    xmin = *std::min_element(x, x + 3);
    ymin = *std::min_element(y, y + 3);
    xmax = *std::max_element(x, x + 3);
    ymax = *std::max_element(y, y + 3);
}
void Renderer::renderTriangle(Triangle &t, std::vector<std::vector<float>> &zbuffer, Point campos, Shader s)
{

    // Shader s;
    //  Point cam(0.f, 0.f,0.f);
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

    auto edge = [](Point p1, Point p2, Point p3)
    {
        return (p3.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (p3.getY() - p1.getY()) * (p2.getX() - p1.getX());
    };
    for (int i = floor(xmin); i <= floor(xmax); i++)
    {
        zx = z;
        for (int j = floor(ymin); j <= floor(ymax); j++)
        {
            Point p;
            p.setX(i);
            p.setY(j);
            float w1, w2, w3, area;
            w1 = edge(t.getB(), t.getC(), p);
            w2 = edge(t.getC(), t.getA(), p);
            w3 = edge(t.getA(), t.getB(), p);
            area = edge(t.getA(), t.getB(), t.getC());
            if (w1 >= 0 && w2 >= 0 && w3 >= 0 && zx < zbuffer[j][i])
            {
                w1 /= area;
                w2 /= area;
                w3 /= area;

                float R = w1 * iAR + w2 * iBR + w3 * iCR;
                float G = w1 * iAG + w2 * iBG + w3 * iCG;
                float B = w1 * iAB + w2 * iBB + w3 * iCB;
                SDL_SetRenderDrawColor(this->pRenderer, int(255 * R), int(255 * G), int(255 * B), 255);
                SDL_RenderDrawPoint(this->pRenderer, i, j);
                zbuffer[j][i] = zx;
            }
            zx = zx - a / c;
        }
        z = z - b / c;
    }
}
Scene Renderer::transformScene(Camera &camera, Scene &scene, Clipper clip)
{
    PROFILE_FUNCTION();
    Scene s;
    for (int i = 0; i < scene.getNumObjects(); i++)
    {
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

        Object o = scene.getObject(i);
        TriMesh &&proj = camera.worldTransform(o);
        clip.setPlane(pNear, pNearNormal);
        clip.clipObject(proj);
        proj = camera.ndcTransform(proj, this->windowHeight, this->windowWidth);

        proj = camera.viewPortTransform(proj, this->windowHeight, this->windowWidth);

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
        transformedObject.setId(o.getId());
        transformedObject.setShaderId(o.getShaderId());
        s.addObject(transformedObject);
    }
    s.copyShaders(scene);
    return s;
}