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
    viewMatrix.setValue(0,0,xaxis.getX());
    viewMatrix.setValue(0,1,yaxis.getX());
    viewMatrix.setValue(0,2,zaxis.getX());

    viewMatrix.setValue(1,0,zaxis.getY());
    viewMatrix.setValue(1,1,yaxis.getY());
    viewMatrix.setValue(1,2,zaxis.getY());

    viewMatrix.setValue(2,0,xaxis.getZ());
    viewMatrix.setValue(2,1,yaxis.getZ());
    viewMatrix.setValue(2,2,zaxis.getZ());

    viewMatrix.setValue(3,0,-xaxis.dotProduct(eye));
    viewMatrix.setValue(3,1,-yaxis.dotProduct(eye));
    viewMatrix.setValue(3,2,-zaxis.dotProduct(eye));
    viewMatrix.setValue(3,3,1.0);

    return viewMatrix;
}

void Camera::makePointAt()
{

    Matrix rot = this->makerotationZMatrix(yaw);
    Point newTarget;
    rot.multiplyVector(target, &newTarget);

    // Calculate new forward direction
    Point newForward = newTarget - position;
    newForward.normalize();
    //cout << position.getX() << " " << position.getY() << " " << position.getZ() << endl;

    // Calculate new Up direction
    Point a = newForward * up.dotProduct(newForward);
    Point newUp = up - a;
    newUp.normalize();

    // New Right direction is easy, its just cross product
    Point newRight = newUp.crossProduct(newForward);

    // Construct Dimensioning and Translation Matrix
    pointAt.setValue(0, 0, newRight.getX());
    pointAt.setValue(0, 1, newRight.getY());
    pointAt.setValue(0, 2, newRight.getZ());
    pointAt.setValue(0, 3, 0.0f);
    pointAt.setValue(1, 0, newUp.getX());
    pointAt.setValue(1, 1, newUp.getY());
    pointAt.setValue(1, 2, newUp.getZ());
    pointAt.setValue(1, 3, 0.0);
    pointAt.setValue(2, 0, newForward.getX());
    pointAt.setValue(2, 1, newForward.getY());
    pointAt.setValue(2, 2, newForward.getZ());
    pointAt.setValue(2, 3, 0.0);
    pointAt.setValue(3, 0, position.getX());
    pointAt.setValue(3, 1, position.getY());
    pointAt.setValue(3, 2, position.getZ());
    pointAt.setValue(3, 3, 1.0);
}
void Camera::turn(float _yaw)
{
    yaw = _yaw;
}

void Camera::moveForward(float speed)
{
    Point newForward = target - position;
    position = position + (newForward * speed);
}

void Camera::moveBackward(float speed)
{
    Point newForward = target - position;
    position = position - (newForward * speed);
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

void Camera::transform(Point in, Point *out)
{
    
    Matrix fpsMatrix = this->makeFPSTransform(position, pitch, yaw);
    //fpsMatrix.invertRotMatrix(fpsMatrix);
    fpsMatrix.multiplyVector(in, out);


    //this->makePointAt();
    //pointAt.invertRotMatrix(pointAt);
    //pointAt.multiplyVector(in, out);
}
Point Camera::getPosition()
{
    return position;
}
TriMesh Camera::worldTransform(TriMesh in)
{
    TriMesh out;
    for (auto tri : in.getTriangles())
    {
        Point p1, p2, p3;
        Triangle t;
        t.setLum(tri.getLum());
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
TriMesh Camera::ndcTransform(TriMesh in, int height, int width)
{
    TriMesh out;
    Matrix matProj;
    // Projection Matrix
    float fNear = 0.1;
    float fFar = 1000.0;
    float fFov = 90.0;
    float fAspectRatio = float(width) / float(height);
    float fFovRad = 1.0 / tanf(fFov * 0.5 / 180.0 * 3.14159);

    /*matProj.setValue(0, 0, fFovRad / fAspectRatio);
    matProj.setValue(1, 1, fFovRad);
    matProj.setValue(2, 2, (fFar + fNear) / (fFar - fNear));
    matProj.setValue(3, 2, (2.0 * fFar * fNear) / (fNear - fFar));
    matProj.setValue(2, 3, 1.0);
    matProj.setValue(3, 3, 0.0);*/

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

        // Project to view and scale for perspective
        matProj.multiplyVector(tri.getA(), &p);
        //p = p / p.getW();
        t.setA(p);
        matProj.multiplyVector(tri.getB(), &p);
        //p = p / p.getW();
        t.setB(p);
        matProj.multiplyVector(tri.getC(), &p);
        //p = p / p.getW();
        t.setC(p);
        out.addTriangle(t);
    }
    return out;
}

TriMesh Camera::viewPortTransform(TriMesh in, int height, int width)
{
    TriMesh out;
    in.sortZ();
    for (auto tri : in.getTriangles())
    {
        Point vCam = tri.getA() - position;
        vCam.normalize();
        tri.computeNormal();

        if (vCam.dotProduct(tri.getNormal()) < 0)
        {
            Triangle t;
            Point p;
            t.setLum(tri.getLum());
            //scale to window size

            /*p.setX((tri.getA().getX() + 1) * 0.5 * float(width));
            p.setY((tri.getA().getY() + 1) * 0.5 * float(height));
            t.setA(p);

            p.setX((tri.getB().getX() + 1) * 0.5 * float(width));
            p.setY((tri.getB().getY() + 1) * 0.5 * float(height));
            t.setB(p);

            p.setX((tri.getC().getX() + 1) * 0.5 * float(width));
            p.setY((tri.getC().getY() + 1) * 0.5 * float(height));
            t.setC(p);*/

            p.setX(tri.getA().getX() * float(width)/(2.0*tri.getA().getW())+(float(width)/2.0));
            p.setY(tri.getA().getY() * float(height)/(2.0*tri.getA().getW())+(float(height)/2.0));
            t.setA(p);

            p.setX(tri.getB().getX() * float(width)/(2.0*tri.getB().getW())+(float(width)/2.0));
            p.setY(tri.getB().getY() * float(height)/(2.0*tri.getB().getW())+(float(height)/2.0));
            t.setB(p);

            p.setX(tri.getC().getX() * float(width)/(2.0*tri.getC().getW())+(float(width)/2.0));
            p.setY(tri.getC().getY() * float(height)/(2.0*tri.getC().getW())+(float(height)/2.0));
            t.setC(p);

            out.addTriangle(t);
        }
    }
    return out;
}