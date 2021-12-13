#pragma once
#include "Point.h"
#include "TriMesh.h"
#include "Matrix.h"

class Camera
{
private:
    Point position;
    Point target;
    Point forward;
    Point up;
    float yaw;
    float pitch;
    Matrix pointAt;

public:
    Camera();
    void makePointAt();
    void turn(float speed);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveLeft(float speed);
    void moveRight(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    Matrix makerotationZMatrix(float rot);
    Matrix makerotationYMatrix(float rot);
    Matrix makeFPSTransform(Point position, float pitch, float yaw);

    void transform(Point &in);
    Point getPosition();
    void worldTransform(TriMesh& in);
    void ndcTransform(TriMesh& in, int height, int width);
    void viewPortTransform(TriMesh& in, int height, int width);
};