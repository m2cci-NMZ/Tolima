#include "Camera.h"
#include <iostream>
#include <math.h>

using namespace std;

Camera::Camera()
{
    Point _target(0, 0, 1);
    Point _up(0, 1, 0);
    Point _position(0, 0, 0);
    up = _up;
    target = _target;
    forward = _target;
    position = _position;
    Matrix pointAt();
    yaw = 0.0;
    pitch = 0.0;
}

Matrix Camera::makeFPSTransform(Point eye, float pitch, float yaw)
{
    float cosPitch = cosf(pitch);
    float sinPitch = sinf(pitch);
    float cosYaw = cosf(yaw);
    float sinYaw = sinf(yaw);
    Point xaxis;
    Point yaxis;
    Point zaxis;

    xaxis.setX(cosYaw);
    xaxis.setZ(-sinYaw);
    yaxis.setX(sinYaw * sinPitch);
    yaxis.setY(cosPitch);
    yaxis.setZ(cosYaw * sinPitch);
    zaxis.setX(sinYaw * cosPitch);
    zaxis.setY(-sinPitch);
    zaxis.setZ(cosPitch * cosYaw);

    Matrix viewMatrix;
    viewMatrix.setValue(0, 0, xaxis.getX());
    viewMatrix.setValue(0, 1, yaxis.getX());
    viewMatrix.setValue(0, 2, zaxis.getX());

    viewMatrix.setValue(1, 0, zaxis.getY());
    viewMatrix.setValue(1, 1, yaxis.getY());
    viewMatrix.setValue(1, 2, zaxis.getY());

    viewMatrix.setValue(2, 0, xaxis.getZ());
    viewMatrix.setValue(2, 1, yaxis.getZ());
    viewMatrix.setValue(2, 2, zaxis.getZ());

    viewMatrix.setValue(3, 0, -xaxis.dotProduct(eye));
    viewMatrix.setValue(3, 1, -yaxis.dotProduct(eye));
    viewMatrix.setValue(3, 2, -zaxis.dotProduct(eye));
    viewMatrix.setValue(3, 3, 1.0);

    return viewMatrix;
}

void Camera::turn(float speed)
{
    yaw += speed;
    Matrix rot = this->makerotationYMatrix(yaw);
    rot.multiplyVector(forward, &target);
}

void Camera::moveForward(float speed)
{
    position = position + (target * speed);
}

void Camera::moveBackward(float speed)
{
    position = position - (target * speed);
}

void Camera::moveLeft(float speed)
{
    Point left;
    left = up.crossProduct(target);
    left.normalize();
    position = position + (left * speed);
}

void Camera::moveRight(float speed)
{
    Point right;
    right = target.crossProduct(up);
    right.normalize();
    position = position + (right * speed);
}

void Camera::moveUp(float speed)
{
    Point up;
    up.setY(1.0);
    position = position + (up * speed);
}

void Camera::moveDown(float speed)
{
    Point up;
    up.setY(-1.0);
    position = position + (up * speed);
}

Matrix Camera::makerotationZMatrix(float rot)
{

    Matrix matrix;
    matrix.setValue(0, 0, cosf(rot));
    matrix.setValue(0, 2, sinf(rot));
    matrix.setValue(2, 0, -sinf(rot));
    matrix.setValue(1, 1, 1.0);
    matrix.setValue(2, 2, cosf(rot));
    matrix.setValue(3, 3, 1.0);
    return matrix;
}

Matrix Camera::makerotationYMatrix(float rot)
{

    Matrix matrix;
    matrix.setValue(0, 0, cosf(rot));
    matrix.setValue(2, 0, sinf(rot));
    matrix.setValue(0, 2, -sinf(rot));
    matrix.setValue(1, 1, 1.0);
    matrix.setValue(2, 2, cosf(rot));
    matrix.setValue(3, 3, 1.0);
    return matrix;
}

void Camera::transform(Point in, Point *out)
{

    Matrix fpsMatrix = this->makeFPSTransform(position, pitch, yaw);
    fpsMatrix.multiplyVector(in, out);
}
Point Camera::getPosition()
{
    return position;
}
TriMesh Camera::worldTransform(TriMesh &in)
{
    TriMesh out;
    for (auto tri : in.getTriangles())
    {
        Point p1, p2, p3;
        Triangle t;
        t.setLum(tri.getLum());
        t.setNA(tri.getNA());
        t.setNB(tri.getNB());
        t.setNC(tri.getNC());
        this->transform(tri.getA(), &p1);
        t.setA(p1);
        this->transform(tri.getB(), &p2);
        t.setB(p2);
        this->transform(tri.getC(), &p3);
        t.setC(p3);
        out.addTriangle(t);
    }
    return out;
}
TriMesh Camera::ndcTransform(TriMesh &in, int height, int width)
{
    TriMesh out;
    Matrix matProj;
    // Projection Matrix
    float fNear = 0.1;
    float fFar = 1000.0;
    float fFov = 90.0;
    float fAspectRatio = float(width) / float(height);
    float fFovRad = 1.0 / tanf(fFov * 0.5 / 180.0 * 3.14159);

    matProj.setValue(0, 0, fFovRad / fAspectRatio);
    matProj.setValue(1, 1, fFovRad);
    matProj.setValue(2, 2, (fFar + fNear) / (fFar - fNear));
    matProj.setValue(2, 3, (2.0 * fFar * fNear) / (fNear - fFar));
    matProj.setValue(3, 2, -1.0);
    matProj.setValue(3, 3, 0.0);

    for (auto tri : in.getTriangles())
    {
        Point p;
        Triangle t;
        t.setLum(tri.getLum());
        t.setNA(tri.getNA());
        t.setNB(tri.getNB());
        t.setNC(tri.getNC());

        // Project to view
        matProj.multiplyVector(tri.getA(), &p);

        t.setA(p);
        matProj.multiplyVector(tri.getB(), &p);

        t.setB(p);
        matProj.multiplyVector(tri.getC(), &p);

        t.setC(p);
        out.addTriangle(t);
    }
    return out;
}

TriMesh Camera::viewPortTransform(TriMesh &in, int height, int width)
{
    TriMesh out;
    in.sortZ();
    for (auto tri : in.getTriangles())
    {
        Point vCam = tri.getA() - position;
        vCam.normalize();
        //tri.computeNormal();

        if (vCam.dotProduct(tri.getNormal()) < 0)
        //if (true)
        {
            Triangle t;
            Point p;
            t.setLum(tri.getLum());
            t.setNA(tri.getNA());
            t.setNB(tri.getNB());
            t.setNC(tri.getNC());
            //scale to window size

            p.setX(tri.getA().getX() * float(width) / (2.0 * tri.getA().getW()) + (float(width) / 2.0));
            p.setY(tri.getA().getY() * float(height) / (2.0 * tri.getA().getW()) + (float(height) / 2.0));
            t.setA(p);

            p.setX(tri.getB().getX() * float(width) / (2.0 * tri.getB().getW()) + (float(width) / 2.0));
            p.setY(tri.getB().getY() * float(height) / (2.0 * tri.getB().getW()) + (float(height) / 2.0));
            t.setB(p);

            p.setX(tri.getC().getX() * float(width) / (2.0 * tri.getC().getW()) + (float(width) / 2.0));
            p.setY(tri.getC().getY() * float(height) / (2.0 * tri.getC().getW()) + (float(height) / 2.0));
            t.setC(p);

            out.addTriangle(t);
        }
    }
    return out;
}