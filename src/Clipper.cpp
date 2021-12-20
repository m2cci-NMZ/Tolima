#include "Clipper.h"
#include "../include/Instrumentor.h"

Clipper::Clipper()
{
}

void Clipper::setPlane(const Point& _plane, const Point& _normal)
{
    plane = _plane;
    normal = _normal;
}

float Clipper::distanceToPlane(Point p)
{
    float d;
    d = normal.dotProduct(p) - normal.dotProduct(plane);
    return d;
}

Point Clipper::intersectPlane(Point p1, Point p2)
{
    Point diff = p1 - plane;
    float prod1 = diff.dotProduct(normal);
    Point line = p2 - p1;
    float prod2 = line.dotProduct(normal);
    float prod3 = prod1 / prod2;
    return p1 - line * prod3;
}

int Clipper::clipAgainstPlane(Triangle& in)
{
    float d1 = this->distanceToPlane(in.getA());
    float d2 = this->distanceToPlane(in.getB());
    float d3 = this->distanceToPlane(in.getC());
    t1 = in;
    t2 = in;
    //everything is in the plane, return 1 and put original triangle in t1
    if (d1 > 0 && d2 > 0 && d3 > 0)
    {
        t1 = in;
        return 1;
    }
    //everything is outside the plane, just return 0
    else if (d1 < 0 && d2 < 0 && d3 < 0)
    {
        return 0;
    }
    //one point outside, triangle must be clipped to form 2 new triangles t1 and t2 and return 2
    else if (d1 * d2 * d3 < 0)
    {
        if (d1 < 0)
        {
            Point&& p1 = this->intersectPlane(in.getB(), in.getA());
            Point&& p2 = this->intersectPlane(in.getC(), in.getA());

            t1.setA(p1);
            t1.setB(in.getB());
            t1.setC(in.getC());

            t2.setA(p1);
            t2.setB(in.getC());
            t2.setC(p2);
        }
        else if (d2 < 0)
        {
            Point&& p1 = this->intersectPlane(in.getC(), in.getB());
            Point&& p2 = this->intersectPlane(in.getA(), in.getB());

            t1.setA(p1);
            t1.setB(in.getC());
            t1.setC(in.getA());

            t2.setA(p1);
            t2.setB(in.getA());
            t2.setC(p2);
        }
        else
        {
            Point&& p1 = this->intersectPlane(in.getA(), in.getC());
            Point&& p2 = this->intersectPlane(in.getB(), in.getC());

            t1.setA(p1);
            t1.setB(in.getA());
            t1.setC(in.getB());

            t2.setA(p1);
            t2.setB(in.getB());
            t2.setC(p2);
        }
        return 2;
    }
    //two points outside,
    else
    {
        if (d1 > 0)
        {
            Point&& p1 = this->intersectPlane(in.getA(), in.getB());
            Point&& p2 = this->intersectPlane(in.getA(), in.getC());

            t1.setA(in.getA());
            t1.setB(p1);
            t1.setC(p2);
        }
        else if (d2 > 0)
        {
            Point&& p1 = this->intersectPlane(in.getA(), in.getB());
            Point&& p2 = this->intersectPlane(in.getB(), in.getC());

            t1.setA(p1);
            t1.setB(in.getB());
            t1.setC(p2);
        }
        else
        {
            Point&& p1 = this->intersectPlane(in.getB(), in.getC());
            Point&& p2 = this->intersectPlane(in.getA(), in.getC());

            t1.setA(p2);
            t1.setB(p1);
            t1.setC(in.getC());
        }
        return 1;
    }
    return 0;
}

void Clipper::clipObject(TriMesh& object)
{
    PROFILE_FUNCTION();
    TriMesh out;
    for (auto& tri : object.getTriangles())
    {
        switch (this->clipAgainstPlane(tri))
        {
        case 0:
            break;
        case 1:
            out.addTriangle(t1);
            break;
        case 2:
            out.addTriangle(t1);
            out.addTriangle(t2);
            break;
        }
    }
    object.setMesh(out);
}
