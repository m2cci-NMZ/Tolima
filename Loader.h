#pragma once
#include <fstream>

#include "TriMesh.h"

using namespace std;

class Loader
{

private:
    string filename;
    TriMesh *object;

public:
    Loader(string _filename, TriMesh *_object);
    bool loadMeshFromFile();
    vector<vector<int>> separateFaceElements(string s);
    int analyzeLine(string line);
    void extractPoint(vector<Point> &verts, string line);
    void addTriangles(const vector<Point> &verts, const vector<Point> &normals, const vector<Point> &textures, const vector<vector<int>> &indices);
};