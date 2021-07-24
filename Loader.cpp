#include "Loader.h"
#include "TriMesh.h"
#include "Point.h"
#include <algorithm>
#include <fstream>
#include <strstream>

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

    // Store all vertices
    vector<Point> verts;
    // Store all normals
    vector<Point> normals;

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream s;
        s << line;

        char junk;

        if (line[0] == 'v' || line[1] != 'n')
        {
            Point p;
            float x, y, z;
            s >> junk >> x >> y >> z;
            p.setX(x);
            p.setY(y);
            p.setZ(z);
            verts.push_back(p);
        }
        if (line[0] == 'v' || line[1] == 'n')
        {
            Point p;
            float x, y, z;
            s >> junk >> x >> y >> z;
            p.setX(x);
            p.setY(y);
            p.setZ(z);
            normals.push_back(p);
        }
        //reconstruct all triangles from vertices
        if (line[0] == 'f')
        {
            string f[3];
            int v[3];
            int vn[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            string sep = "//";
            if (f[0].find(sep) != string::npos)
            {
                for (int i = 0; i < 3; i++)
                {
                    int *els = this->separateFaceElements(f[i]);
                    v[i] = els[0];
                    vn[i] = els[1];
                }
                Triangle t(verts[v[0] - 1], verts[v[1] - 1], verts[v[2] - 1]);
                object->addTriangle(t);
            }
            else
            {
                Triangle t(verts[stoi(f[0]) - 1], verts[stoi(f[1]) - 1], verts[stoi(f[2]) - 1]);
                object->addTriangle(t);
            }
        }
    }

    f.close();

    return true;
}

int *Loader::separateFaceElements(string s)
{
    int elements[2];
    int sep = s.find("//");
    int end = s.size();
    elements[0] = stoi(s.substr(0, sep));
    elements[1] = stoi(s.substr(sep, end));
    return elements;
}
int Loader::analyzeLine(string s)
{
    if (s.substr(0,2) == "f "){
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
    else{
        return 0;
    }
}
