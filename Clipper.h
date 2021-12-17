#pragma once
#include "Point.h"
#include "Triangle.h"
#include "TriMesh.h"

class Clipper
{
private:
    Point plane;
    Point normal;
    Triangle t1;
    Triangle t2;

public:
    Clipper();
    void setPlane(const Point& _plane, const Point& _normal);
    float distanceToPlane(Point p);
    Point intersectPlane(Point p1, Point p2);
    int clipAgainstPlane(Triangle& in);
    void clipObject(TriMesh& in);
};