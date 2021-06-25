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
    void setPlane(Point _plane, Point _normal);
    float distanceToPlane(Point p);
    Point intersectPlane(Point p1, Point p2);
    int clipAgainstPlane(Triangle in);
    TriMesh clipObject(TriMesh in);
};