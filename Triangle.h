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

    Point getNA();
    Point getNB();
    Point getNC();

    void setNA(const Point &_na);
    void setNB(const Point &_nb);
    void setNC(const Point &_nc);

    void setA(const Point &_a);
    void setB(const Point &_b);
    void setC(const Point &_c);

    void setLum(float _lum);
    float getLum();

    void computeNormal();
    Point getNormal();
    void derivePlane(float &a, float &b, float &c, float &d);

    ~Triangle();
};