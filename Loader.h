#pragma once
#include <fstream>

#include "TriMesh.h"
#include "Scene.h"

using namespace std;

class Loader
{

private:
    string _filename;
    string _shaderfname;
    Scene *_scene;
    vector<Point> verts;
    vector<Point> normals;
    vector<Point> vtextures;

public:
    Loader(const string filename,const string shaderfname, Scene *scene);
    bool loadMeshFromFile();
    void loadObjects(const std::vector<string> &data);
    void loadObject(const std::vector<string> &data, int index);
    vector<vector<int>> separateFaceElements(const std::vector<string> &data, int i);
    int analyzeLine(string line);
    void extractPoint(vector<Point> &verts, const std::vector<string> &data, int i);
    void addTriangles(const std::vector<Point> &verts, const std::vector<Point> &normals, const std::vector<Point> &textures, const std::vector<vector<int>> &indices, Object &object);
    bool readMtl(string fname);
    void loadShaders(const std::vector<string> &data);
    void loadShader(const std::vector<string> &data, int index);
    int parseMtl(const string input);
    void loadVertices(const std::vector<string> &data);
};