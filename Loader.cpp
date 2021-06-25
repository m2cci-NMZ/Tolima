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

    while (!f.eof())
    {
        char line[128];
        f.getline(line, 128);

        strstream s;
        s << line;

        char junk;

        if (line[0] == 'v')
        {
            Point p;
            float x, y, z;
            s >> junk >> x >> y >> z;
            p.setX(x);
            p.setY(y);
            p.setZ(z);
            verts.push_back(p);
        }
        //reconstruct all triangles from vertices
        if (line[0] == 'f')
        {
            int f[3];
            s >> junk >> f[0] >> f[1] >> f[2];
            Triangle t(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]);
            object->addTriangle(t);
        }
    }

    f.close();

    return true;
}
