#pragma once
#include "Point.h"

class Triangle
{
private:
    Point a;
    Point b;
    Point c;
    Point nA;
    Point nB;
    Point nC;
    float light;

public:
    Triangle();
    Triangle(Point _a, Point _b, Point _c);

    Point getA();
    Point getB();
    Point getC();

    void setA(const Point& _a);
    void setB(const Point& _b);
    void setC(const Point& _c);

    void setLum(float _lum);
    float getLum();

    void computeNormal();
    Point getNormal();


    ~Triangle();
};