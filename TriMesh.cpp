#include "TriMesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "Instrumentor.h"

using namespace std;

TriMesh::TriMesh()
{
    triangles = vector<Triangle>();
    nTriangles = 0;
}

TriMesh::TriMesh(vector<Triangle> _triangles)
{
    triangles = _triangles;
}

vector<Triangle>& TriMesh::getTriangles()
{
    return triangles;
}

Triangle TriMesh::getTriangle(int i)
{
    PROFILE_FUNCTION();
    return triangles.at(i);
}

int TriMesh::getnTriangles()
{
    return nTriangles;
}
void TriMesh::addTriangle(Triangle _triangle)
{
    triangles.push_back(_triangle);
    nTriangles++;
}

TriMesh TriMesh::copy()
{
    TriMesh TriMeshCopy;
    for (auto triangle : triangles)
    {
        TriMeshCopy.addTriangle(triangle);
    }
    TriMeshCopy.nTriangles = nTriangles;
    return TriMeshCopy;
}

void TriMesh::createCube()
{

    float mat[12][9] = {

        //// SOUTH
        {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},

        //// EAST
        {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f},

        //// NORTH
        {1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f},

        //// WEST
        {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f},

        //// TOP
        {0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f},

        //// BOTTOM
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f},

    };
    for (auto tri : mat)
    {
        Point p1 = Point(tri[0], tri[1], tri[2]);
        Point p2 = Point(tri[3], tri[4], tri[5]);
        Point p3 = Point(tri[6], tri[7], tri[8]);

        Triangle triangle = Triangle(p1, p2, p3);

        addTriangle(triangle);
    }
}

void TriMesh::sortZ()
{
    if (&triangles)
    {
        sort(triangles.begin(), triangles.end(), [](Triangle t1, Triangle t2)
             {
                 float z1 = (t1.getA().getZ() + t1.getB().getZ() + t1.getC().getZ()) / 3.0f;
                 float z2 = (t2.getA().getZ() + t2.getB().getZ() + t2.getC().getZ()) / 3.0f;
                 return z1 > z2;
             });
    }
}
void TriMesh::setMesh(const TriMesh& mesh){
    triangles = mesh.triangles;
}

TriMesh::~TriMesh()
{
    /*if (position)
        delete position;
    for (unsigned long int i = 0; i < triangles->size(); i++)
    {
        if (triangles->at(i))
            delete triangles->at(i);
    }

    if (triangles)
        delete triangles;*/
}