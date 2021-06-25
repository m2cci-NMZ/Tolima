#include "Point.h"
#include <math.h>

Point::Point()
{
    x = 0.0;
    y = 0.0;
    z = 0.0;
}
Point::Point(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
}
float Point::getX()
{
    return x;
}
float Point::getY()
{
    return y;
}
float Point::getZ()
{
    return z;
}
float Point::getW()
{
    return w;
}
void Point::setX(float _x)
{
    x = _x;
}
void Point::setY(float _y)
{
    y = _y;
}
void Point::setZ(float _z)
{
    z = _z;
}
void Point::setW(float _w)
{
    w = _w;
}
Point Point::operator+(const Point &p1)
{
    Point p3;
    p3.x = this->x + p1.x;
    p3.y = this->y + p1.y;
    p3.z = this->z + p1.z;
    return p3;
}
Point Point::operator-(const Point &p1)
{
    Point p3;
    p3.x = this->x - p1.x;
    p3.y = this->y - p1.y;
    p3.z = this->z - p1.z;
    return p3;
}

Point Point::operator*(float a)
{
    Point p3;
    p3.x = this->x * a;
    p3.y = this->y * a;
    p3.z = this->z * a;
    return p3;
};

Point Point::operator/(float a)
{
    Point p3;
    p3.x = this->x / a;
    p3.y = this->y / a;
    p3.z = this->z / a;
    return p3;
};

Point Point::crossProduct(Point p1)
{
    Point p3;

    p3.x = this->y * p1.z - this->z * p1.y;
    p3.y = this->z * p1.x - this->x * p1.z;
    p3.z = this->x * p1.y - this->y * p1.x;

    return p3;
}
float Point::norm()
{
    return sqrtf(x * x + y * y + z * z);
}
void Point::normalize()
{
    float norm = sqrtf(x * x + y * y + z * z);
    x /= norm;
    y /= norm;
    z /= norm;
}
float Point::dotProduct(Point p){
    float dot;
    dot = this->x*p.x + this->y*p.y + this->z*p.z;
    return dot;
}

Point::~Point()
{
}