#pragma once
class Point
{
private:
    float x = 0;
    float y = 0;
    float z = 0;
    float w = 1;

public:
    Point(float _x, float _y, float _z);
    Point();

    float getX();
    float getY();
    float getZ();
    float getW();

    void setX(float _x);
    void setY(float _Y);
    void setZ(float _Z);
    void setW(float _w);

    Point operator+(const Point &_point);
    Point operator-(const Point &_point);
    Point operator*(float a);
    Point operator/(float a);
    Point crossProduct(const Point &p);
    float dotProduct(const Point &p);
    float norm();
    void normalize();

    ~Point(void);
};
