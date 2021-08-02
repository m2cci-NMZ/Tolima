#include "Loader.h"
#include "TriMesh.h"
#include "Point.h"
#include <algorithm>
#include <fstream>
#include <strstream>
#include <sstream>
#include <iostream>

using namespace std;

Loader::Loader(string _filename, TriMesh *_object)
{
    filename = _filename;
    object = _object;
}

bool Loader::loadMeshFromFile()
{
    ifstream f(filename);
    if (!f.is_open())
        return false;

    // vector containing vertex coordinates
    vector<Point> verts;
    // vector containing normal coordinates
    vector<Point> normals;
    // vector containing texture coordinates
    vector<Point> vtextures;

    // vector containing indices
    vector<vector<int>> vind;

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream s;
        s << line;

        switch (this->analyzeLine(line))
        {
        case 0:
            break;
        case 1:
            vind = this->separateFaceElements(line);
            this->addTriangles(verts, normals, vtextures, vind);
            break;
        case 2:
            this->extractPoint(verts, line);
            break;
        case 3:
            this->extractPoint(normals, line);
            break;
        case 4:
            this->extractPoint(vtextures, line);
            break;
        }
    }

    f.close();

    return true;
}

vector<vector<int>> Loader::separateFaceElements(string s)
{
    vector<vector<int>> elements;
    istringstream ss(s);
    char del = '/';
    string token;
    string f;
    while (ss >> f)
    {
        vector<int> indices;
        if (f[0] != 'f')
        {
            istringstream stmp(f);
            while (getline(stmp, token, del))
            {
                indices.push_back(stoi(token));
            }
            elements.push_back(indices);
        }
    }

    return elements;
}
int Loader::analyzeLine(string s)
{
    if (s.substr(0, 2) == "f ")
    {
        return 1;
    }
    if (s.substr(0, 2) == "v ")
    {
        return 2;
    }
    if (s.substr(0, 2) == "vn")
    {
        return 3;
    }
    if (s.substr(0, 2) == "vt")
    {
        return 4;
    }
    else
    {
        return 0;
    }
}
void Loader::extractPoint(vector<Point> &verts, string line)
{
    strstream s;
    string junk;
    Point p;
    float x, y, z;

    s << line;
    s >> junk >> x >> y >> z;
    p.setX(x);
    p.setY(y);
    p.setZ(z);
    verts.push_back(p);
}
void Loader::addTriangles(const vector<Point> &verts, const vector<Point> &normals, const vector<Point> &textures, const vector<vector<int>> &indices)
{
    int nbVerts = indices.size();
    switch (nbVerts % 3)
    {
    case 0:
        for (int i = 0; i < nbVerts / 3; i++)
        {
            int idx1, idx2, idx3;
            idx1 = indices.at(i * 3).at(0) - 1;
            idx2 = indices.at(i * 3 + 1).at(0) - 1;
            idx3 = indices.at(i * 3 + 2).at(0) - 1;
            Triangle t;
            t.setA(verts.at(idx1));
            t.setB(verts.at(idx2));
            t.setC(verts.at(idx3));
            object->addTriangle(t);
        };
        break;
    }
}
