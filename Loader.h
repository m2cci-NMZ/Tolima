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
    int* separateFaceElements(string s); 
    int analyzeLine(string line);
    void extractPoint(vector<Point>& verts, string line);
};