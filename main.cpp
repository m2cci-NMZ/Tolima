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

using namespace std;

void fillTriangle(SDL_Renderer *p, Point p1, Point p2, Point p3)
{
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
            SDL_RenderDrawLine(p, p3.getX(), p3.getY(), tx, ty);
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
            SDL_RenderDrawLine(p, p1.getX(), p1.getY(), tx, ty);
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
            SDL_RenderDrawLine(p, p2.getX(), p2.getY(), tx, ty);
            tx = tx + vx;
            ty = ty + vy;
            counter = counter + 1;
        }
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Window *pWindow{nullptr};
    SDL_Renderer *pRenderer{nullptr};
    if (SDL_CreateWindowAndRenderer(720, 480, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }
    int width;
    int height;
    SDL_GetWindowSize(pWindow, &width, &height);

    SDL_Event events;
    bool isOpen{true};
    Point p1(1.0, 1.0, 1.0);

    TriMesh cube;

    Camera camera;

    Shader shader;

    string f = "teapot.obj";
    Loader loader(f, &cube);
    if (!loader.loadMeshFromFile())
    {
        cube.readfile("exemple.txt");
    }

    uint32_t startTime = SDL_GetTicks();
    double elapsedTime = 0;
    uint32_t counter = 0;

    float fYaw = 0;
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

    Clipper clip;

    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            case SDL_QUIT:
                isOpen = false;
                break;
            case SDL_KEYDOWN:
                switch (events.key.keysym.sym)
                {
                case SDLK_q:
                    fYaw -= elapsedTime * 0.0002;
                    camera.turn(fYaw);
                    break;
                case SDLK_z:
                    camera.moveForward(0.1);
                    break;
                case SDLK_s:
                    camera.moveBackward(0.1);
                    break;
                case SDLK_d:
                    fYaw += elapsedTime * 0.0002;
                    camera.turn(fYaw);
                    break;
                case SDLK_x:
                    camera.moveDown(0.1);
                    break;
                case SDLK_w:
                    camera.moveUp(0.1);
                    break;
                }

                break;
            }
        }

        if (counter > 1000)
        {
            counter = 0;
            startTime = SDL_GetTicks();
        }

        TriMesh cubeProj;
        for (auto tri : cube.getTriangles())
        {
            Point toto(0, 0, 20);
            tri.setA(tri.getA() + toto);
            tri.setB(tri.getB() + toto);
            tri.setC(tri.getC() + toto);
            cubeProj.addTriangle(tri);
        } 
        cubeProj = shader.computeShader(cubeProj);
        cubeProj = camera.worldTransform(cubeProj);
        clip.setPlane(pNear, pNearNormal);
        cubeProj = clip.clipObject(cubeProj);
        cubeProj = camera.ndcTransform(cubeProj, height, width);

        cubeProj = camera.viewPortTransform(cubeProj, height, width);
        //cout << cubeProj.getnTriangles() <<endl;
                
        clip.setPlane(pLeft, pLeftNormal);
        cubeProj = clip.clipObject(cubeProj);

        clip.setPlane(pUp, pUpNormal);
        cubeProj = clip.clipObject(cubeProj);

        clip.setPlane(pRight, pRightNormal);
        cubeProj = clip.clipObject(cubeProj);

        clip.setPlane(pDown, pDownNormal);
        cubeProj = clip.clipObject(cubeProj);

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);
        SDL_RenderClear(pRenderer);

        for (auto tri : cubeProj.getTriangles())
        {

            // Draw Triangles
            SDL_SetRenderDrawColor(pRenderer, int(255 * tri.getLum()), int(255 * tri.getLum()), int(255 * tri.getLum()), 255);
            fillTriangle(pRenderer, tri.getA(), tri.getB(), tri.getC());
            SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);

            //Draw lines to see wireframe
           /* SDL_SetRenderDrawColor(pRenderer, 100, 100, 100, 255);
            SDL_RenderDrawLine(pRenderer, int(tri.getA().getX()), int(tri.getA().getY()), int(tri.getB().getX()), int(tri.getB().getY()));
            SDL_RenderDrawLine(pRenderer, int(tri.getB().getX()), int(tri.getB().getY()), int(tri.getC().getX()), int(tri.getC().getY()));
            SDL_RenderDrawLine(pRenderer, int(tri.getC().getX()), int(tri.getC().getY()), int(tri.getA().getX()), int(tri.getA().getY()));
        */
        }
        SDL_RenderPresent(pRenderer);
        //SDL_Delay(1);
        //cout<<"yaw:"<<fYaw<<endl;

        uint32_t currTime = SDL_GetTicks();
        elapsedTime = (currTime - startTime) / 1000.0;
        counter++;
        string title;

        title = to_string(double(counter) / elapsedTime);
        title.append(" FPS");
        const char *titleConverted = title.c_str();
        SDL_SetWindowTitle(pWindow, titleConverted);
        //cout << double(counter) / elapsedTime << " FPS" << endl;

        //translater += 0.001;
    }
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return EXIT_SUCCESS;
}
