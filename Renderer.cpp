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

    for (auto tri : object.getTriangles())
    {
        //SDL_SetRenderDrawColor(this->pRenderer, int(255 * tri.getLum()), int(255 * tri.getLum()), int(255 * tri.getLum()), 255);
        //this->drawTriangle(tri.getA(), tri.getB(), tri.getC());
        this->renderTriangle(tri);
    }
}
int Renderer::closeWindow()
{
    SDL_DestroyRenderer(this->pRenderer);
    SDL_DestroyWindow(this->pWindow);
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
    pRight.setX(float(this->windowWidth));
    pRightNormal.setX(-1.0);

    Point pDown, pDownNormal;
    pDown.setY(this->windowHeight);
    pDownNormal.setY(-1.0);

    uint32_t startTime = SDL_GetTicks();
    double elapsedTime = 0;
    uint32_t counter = 0;

    while (this->isOpen)
    {
        {
            this->eventManager(camera);
        }

        if (counter > 1000)
        {
            counter = 0;
            startTime = SDL_GetTicks();
        }

        //TriMesh proj;

        TriMesh&& proj = camera.worldTransform(object);
        clip.setPlane(pNear, pNearNormal);
        proj = clip.clipObject(proj);
        proj = camera.ndcTransform(proj, this->windowHeight, this->windowWidth);

        proj = camera.viewPortTransform(proj, this->windowHeight, this->windowWidth);

        clip.setPlane(pLeft, pLeftNormal);
        proj = clip.clipObject(proj);

        clip.setPlane(pUp, pUpNormal);
        proj = clip.clipObject(proj);

        clip.setPlane(pRight, pRightNormal);
        proj = clip.clipObject(proj);

        clip.setPlane(pDown, pDownNormal);
        proj = clip.clipObject(proj);
        SDL_SetRenderDrawColor(pRenderer, 125, 125, 125, 0);
        SDL_RenderClear(this->pRenderer);
        this->drawObject(proj);
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
void Renderer::renderTriangle(Triangle &t)
{
    Shader s;
    s.computeVertIntensities(t);
    float xmin, xmax, ymin, ymax;
    this->boundingBox(t, xmin, xmax, ymin, ymax);
    float iA, iB, iC;
    iA = s.getIntensityA();
    iB = s.getIntensityB();
    iC = s.getIntensityC();
    auto edge = [](Point p1, Point p2, Point p3)
    {
        return (p3.getX() - p1.getX()) * (p2.getY() - p1.getY()) - (p3.getY() - p1.getY()) * (p2.getX() - p1.getX());
    };
    for (int i = round(xmin); i <= round(xmax); i++)
    {
        for (int j = round(ymin); j <= round(ymax); j++)
        {
            Point p;
            p.setX(i);
            p.setY(j);
            float w1, w2, w3, area;
            w1 = edge(t.getB(), t.getC(), p);
            w2 = edge(t.getC(), t.getA(), p);
            w3 = edge(t.getA(), t.getB(), p);
            area = edge(t.getA(), t.getB(), t.getC());
            if (w1 >= 0 && w2 >= 0 && w3 >= 0)
            {
                w1 /= area;
                w2 /= area;
                w3 /= area;
                /*
                iA = 1.0;
                iB= 1.0;
                iC= 0.5;
                */
                float color = w1 * iA + w2 * iB + w3 * iC;
                SDL_SetRenderDrawColor(this->pRenderer, int(255 * color), int(255 * color), int(255 * color), 255);
                SDL_RenderDrawPoint(this->pRenderer, i, j);
            }
        }
    }
}