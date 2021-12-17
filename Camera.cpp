#include "Camera.h"
#include <iostream>
#include <math.h>
#include "Instrumentor.h"

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
    target = forward;
    Matrix rot = this->makerotationYMatrix(yaw);
    rot.multiplyVector(target);
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

void Camera::transform(Point &in)
{

    Matrix fpsMatrix = this->makeFPSTransform(position, pitch, yaw);
    fpsMatrix.multiplyVector(in);
}
Point Camera::getPosition()
{
    return position;
}
void Camera::worldTransform(TriMesh &in)
{
    PROFILE_FUNCTION();
    // for (auto& tri : in.getTriangles())
    for (long unsigned int i = 0; i < in.getTriangles().size(); i++)
    {
        //Point vCam = in.getTriangles()[i].getA() - position;
        //vCam.normalize();
        // tri.computeNormal();

        if (target.dotProduct(in.getTriangles()[i].getNormal()) <= 0)
        {
            this->transform(in.getTriangles()[i].getA());
            this->transform(in.getTriangles()[i].getB());
            this->transform(in.getTriangles()[i].getC());
        }
        else
        {
            auto it = in.getTriangles().begin() + i;
            *it = std::move(in.getTriangles().back());
            in.getTriangles().pop_back();
            i--;
        }
    }
}
void Camera::ndcTransform(TriMesh &in, int height, int width)
{
    PROFILE_FUNCTION();
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

    for (auto &tri : in.getTriangles())
    {
        // Project to view
        matProj.multiplyVector(tri.getA());

        matProj.multiplyVector(tri.getB());

        matProj.multiplyVector(tri.getC());
    }
}

void Camera::viewPortTransform(TriMesh &in, int height, int width)
{
    PROFILE_FUNCTION();
    TriMesh out;
    for (auto &tri : in.getTriangles())
    {

        float x, y;

        // scale to window size

        x = tri.getA().getX() * float(width) / (2.0 * tri.getA().getW()) + (float(width) / 2.0);
        y = tri.getA().getY() * float(height) / (2.0 * tri.getA().getW()) + (float(height) / 2.0);
        // z = tri.getA().getZ();
        tri.getA().setX(x);
        tri.getA().setY(y);
        // tri.getA().setZ(z);

        x = tri.getB().getX() * float(width) / (2.0 * tri.getB().getW()) + (float(width) / 2.0);
        y = tri.getB().getY() * float(height) / (2.0 * tri.getB().getW()) + (float(height) / 2.0);
        // z = tri.getB().getZ();
        tri.getB().setX(x);
        tri.getB().setY(y);
        // tri.getB().setZ(z);

        x = tri.getC().getX() * float(width) / (2.0 * tri.getC().getW()) + (float(width) / 2.0);
        y = tri.getC().getY() * float(height) / (2.0 * tri.getC().getW()) + (float(height) / 2.0);
        // z = tri.getC().getZ();
        tri.getC().setX(x);
        tri.getC().setY(y);
        // tri.getC().setZ(z);
    }
}