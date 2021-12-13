#pragma once
#include <vector>
#include <string>
#include "Triangle.h"

using namespace std;

class TriMesh
{
private:
    int nTriangles;
    vector<Triangle > triangles;

public:
    TriMesh();
    TriMesh(vector<Triangle > _triangles);

    TriMesh copy();

    void addTriangle(Triangle _triangle);
    void setMesh(const TriMesh& mesh);

    vector<Triangle >& getTriangles();
    Triangle getTriangle(int i);

    int getnTriangles();

    void createCube();

    void sortZ();

    ~TriMesh();
};