#include "Loader.h"
#include "TriMesh.h"
#include "Point.h"
#include "Scene.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>

// TODO: refactor to have a generic Loader abstract class and then specialize obj and mtl loaders
Loader::Loader(const string filename, const string shaderfname, Scene *scene)
{
    _filename = filename;
    _scene = scene;
    _shaderfname = shaderfname;
}

bool Loader::loadMeshFromFile()
{
    // vector containing indices
    vector<vector<int>> vind;

    vector<string> words;
    ifstream input;
    input.open(_filename);
    if (input.fail())
    {
        return false;
    }
    else
    {
        string word;
        this->readMtl(_shaderfname);
        while (input >> word)
            words.push_back(word);
        this->loadVertices(words);
        this->loadObjects(words);
        return true;
    }
}
void Loader::loadObjects(const std::vector<string> &data)
{
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] == "o")
        {
            this->loadObject(data, i + 1);
        }
        else if (data[i] == "mtllib")
        {
            this->readMtl(data[i + 1]);
        }
    }
}
void Loader::loadVertices(const std::vector<string> &data)
{
    for (int i = 0; i < data.size(); i++)
    {
        switch (this->analyzeLine(data[i]))
        {
        case 2:
            this->extractPoint(verts, data, i);
            break;
        case 3:
            this->extractPoint(normals, data, i);
            break;
        default:
            break;
        }
    }
}
void Loader::loadObject(const std::vector<string> &data, int i)
{
    // vector containing indices
    vector<vector<int>> vind;
    Object o;
    string objectId = data[i];
    o.setId(objectId);
    string mtl_fname;
    while (u_int32_t(i) <= data.size() && data[i] != "o")
    {
        switch (this->analyzeLine(data[i]))
        {
        case 0:
            break;
        case 1:
            vind = this->separateFaceElements(data, i);
            this->addTriangles(verts, normals, vtextures, vind, o);
            break;
        case 5:
            // mtl_fname = data[i + 1];
            // this->readMtl(mtl_fname);
            break;
        case 6:
            if (o.getnTriangles() > 0)
            {
                _scene->addObject(o);
                Object o;
                o.setId(objectId);
            }
            o.setShaderId(data[i + 1]);
            break;
        default:
            break;
        }
        i++;
    }
    if (o.getShaderId() == "")
    {
        // TODO: fix this!
        o.setShaderId(_scene->getObject(_scene->getNumObjects() - 1).getShaderId());
    }
    _scene->addObject(o);
}

vector<vector<int>> Loader::separateFaceElements(const std::vector<string> &data, int i)
{
    vector<vector<int>> elements;
    string T;
    int index = i + 1;
    while (index < data.size() && data[index][0] >= '1' && data[index][0] <= '9')
    {
        std::stringstream X(data[index]);
        vector<int> indices;
        while (std::getline(X, T, '/'))
        {
            if (T != "")
            {
                indices.push_back(stoi(T));
            }
            else
            {
                indices.push_back(0);
            }
        }
        elements.push_back(indices);
        index++;
    }
    return elements;
}
int Loader::analyzeLine(string s)
{
    if (s == "f")
    {
        return 1;
    }
    if (s == "v")
    {
        return 2;
    }
    if (s == "vn")
    {
        return 3;
    }
    if (s == "vt")
    {
        return 4;
    }
    if (s == "mtllib")
    {
        return 5;
    }
    if (s == "usemtl")
    {
        return 6;
    }
    else
    {
        return 0;
    }
}
void Loader::extractPoint(vector<Point> &verts, const std::vector<string> &data, int i)
{
    Point p;

    p.setX(std::stof(data[i + 1]));
    p.setY(std::stof(data[i + 2]));
    p.setZ(std::stof(data[i + 3]));
    verts.push_back(p);
}

void Loader::addTriangles(const vector<Point> &verts, const vector<Point> &normals, const vector<Point> &textures, const vector<vector<int>> &indices, Object &object)
{
    Triangle t;
    int idx1, idx2, idx3;
    int nbVerts = indices.size();
    // Only works for convex polygons
    for (int i = 2; i < nbVerts; i++)
    {
        idx1 = indices.at(0).at(0) - 1;
        idx2 = indices.at(i - 1).at(0) - 1;
        idx3 = indices.at(i).at(0) - 1;
        t.setA(verts.at(idx1));
        t.setB(verts.at(idx2));
        t.setC(verts.at(idx3));
        t.setUntransA(verts.at(idx1));
        t.setUntransB(verts.at(idx2));
        t.setUntransC(verts.at(idx3));

        if (indices.at(0).size() == 3)
        {
            idx1 = indices.at(0).at(2) - 1;
            idx2 = indices.at(i - 1).at(2) - 1;
            idx3 = indices.at(i).at(2) - 1;
            t.setNA(normals.at(idx1));
            t.setNB(normals.at(idx2));
            t.setNC(normals.at(idx3));
        }
        else
        {
            t.computeNormal();
        }
        object.addTriangle(t);
    };
}
bool Loader::readMtl(string fname)
{
    vector<string> words;
    ifstream input;
    input.open(fname);
    if (input.fail())
    {
        return false;
    }
    else
    {
        string word;
        while (input >> word)
            words.push_back(word);
        this->loadShaders(words);
        return true;
    }
}
void Loader::loadShaders(const std::vector<string> &data)
{
    for (std::size_t i = 0; i < data.size(); ++i)
    {
        if (data[i] == "newmtl")
        {
            this->loadShader(data, i + 1);
        }
    }
}
void Loader::loadShader(const std::vector<string> &data, int index)
{
    Shader s;
    s.setId(data[index]);
    while (index < data.size() && data[index] != "newmtl")
    {
        switch (this->parseMtl(data[index]))
        {
        case 0:
            s.setKa(std::stof(data[index + 1]), std::stof(data[index + 2]), std::stof(data[index + 3]));
            break;
        case 1:
            s.setKd(std::stof(data[index + 1]), std::stof(data[index + 2]), std::stof(data[index + 3]));
            break;
        case 2:
            s.setKs(std::stof(data[index + 1]), std::stof(data[index + 2]), std::stof(data[index + 3]));
            break;
        case 3:
            s.setNs(std::stof(data[index + 1]));
            break;
        case 4:
            break;
        }
        index++;
    }
    this->_scene->addShader(s);
}
int Loader::parseMtl(const string input)
{
    if (input == "Ka")
    {
        return 0;
    }
    else if (input == "Kd")
    {
        return 1;
    }
    else if (input == "Ks")
    {
        return 2;
    }
    else if (input == "Ns")
    {
        return 3;
    }
    else
    {
        return 4;
    }
}
