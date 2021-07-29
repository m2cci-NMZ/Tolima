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
    vector<int[3]> vind;


    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream s;
        s << line;
        vector<int[3]> faceElements;

        switch (this->analyzeLine(line))
        {
        case 0:
            break;
        case 1:
            vind = this->separateFaceElements(line);
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

vector<int[3]> Loader::separateFaceElements(string s)
{
    vector<int[3]> elements;
    istringstream ss(s);
    char del = '/';
    string token;
    string f;
    while (ss >> f)
    {
        int indexes[3];
        if (f[0] != 'f')
        {
            istringstream stmp(f);
            int i = 0;
            while (getline(stmp, token, del))
            {
                indexes[i] = stoi(token);
                i++;
            }
            elements.push_back(indexes);
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
