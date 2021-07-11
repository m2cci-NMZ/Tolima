#include <cmath>

#include "Triangle.h"
#include "Point.h"

Triangle::Triangle()
{
    Point a = Point();
    Point b = Point();
    Point c = Point();
    Point normal = Point();
    float light = 1;
}

Triangle::Triangle(Point _a, Point _b, Point _c)
{
    a = _a;
    b = _b;
    c = _c;
}

Point Triangle::getA()
{
    return a;
}

Point Triangle::getB()
{
    return b;
}

Point Triangle::getC()
{
    return c;
}

void Triangle::setA(const Point& _a)
{
    a = _a;
}
void Triangle::setB(const Point& _b)
{
    b = _b;
}
void Triangle::setC(const Point& _c)
{
    c = _c;
}

void Triangle::setLum(float _lum)
{
    light = _lum;
}

float Triangle::getLum()
{
    return light;
}

void Triangle::computeNormal()
{
    Point line1, line2;
    line1 = b - a;
    line2 = c - a;

    normal = line1.crossProduct(line2);
    normal.normalize();
}

Point Triangle::getNormal() {
    return normal;
}

Triangle::~Triangle()
{
    /*if (a)
    {
        delete a;
    }
    if (b)
    {
        delete b;
    }
    if (c)
    {
        delete c;
    }*/
}
