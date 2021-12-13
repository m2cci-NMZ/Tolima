#include <cmath>

#include "Triangle.h"
#include "Point.h"
#include "Instrumentor.h"

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

    untransA = _a;
    untransB = _c;
    untransC = _c;

}
Triangle::Triangle(const Triangle &t)
{
    a = t.a;
    b = t.b;
    c = t.c;

    untransA = t.untransA;
    untransB = t.untransB;
    untransC = t.untransC;

    nA = t.nA;
    nB = t.nB;
    nC = t.nC;
}

Point& Triangle::getA()
{
    return a;
}

Point& Triangle::getB()
{
    return b;
}

Point& Triangle::getC()
{
    return c;
}

Point Triangle::getUntransA()
{
    return untransA;
}

Point Triangle::getUntransB()
{
    return untransB;
}

Point Triangle::getUntransC()
{
    return untransC;
}


Point Triangle::getNA()
{
    return nA;
}

Point Triangle::getNB()
{
    return nB;
}

Point Triangle::getNC()
{
    return nC;
}

void Triangle::setA(const Point &_a)
{
    a = _a;
}
void Triangle::setB(const Point &_b)
{
    b = _b;
}
void Triangle::setC(const Point &_c)
{
    c = _c;
}

void Triangle::setUntransA(const Point &_a)
{
    untransA = _a;
}
void Triangle::setUntransB(const Point &_b)
{
    untransB = _b;
}
void Triangle::setUntransC(const Point &_c)
{
    untransC = _c;
}

void Triangle::setNA(const Point &_nA)
{
    nA = _nA;
    nA.normalize();
}
void Triangle::setNB(const Point &_nB)
{
    nB = _nB;
    nB.normalize();
}
void Triangle::setNC(const Point &_nC)
{
    nC = _nC;
    nC.normalize();
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

    nA = line1.crossProduct(line2);
    nA.normalize();
    nB = nA;
    nC = nA;
}

Point Triangle::getNormal()
{
    Point nMean = (nA + nB + nC) / 3;
    nMean.normalize();
    return nMean;
}

void Triangle::derivePlane(float &A, float &B, float &C, float &D)
{
    PROFILE_FUNCTION();
    Point d0 = this->b - this->a;
    Point d1 = this->c - this->a;
    Point normal;

    normal = d0.crossProduct(d1);
    normal.normalize();
    A = normal.getX();
    B = normal.getY();
    C = normal.getZ();

    D = -(A * this->a.getX() + B * this->a.getY() + C * this->a.getZ());
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
