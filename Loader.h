#pragma once
#include <fstream>

#include "TriMesh.h"
#include "Scene.h"

using namespace std;

class Loader
{

private:
    string _filename;
    Scene _scene;

public:
    Loader(const string filename, Scene& scene);
    bool loadMeshFromFile();
    void loadObjects(const std::vector<string>& data);
    void loadObject(const std::vector<string>& data, int index);
    vector<vector<int>> separateFaceElements(const std::vector<string> &data, int i);
    int analyzeLine(string line);
    void extractPoint(vector<Point> &verts, const std::vector<string> &data, int i);
    void addTriangles(const std::vector<Point> &verts, const std::vector<Point> &normals, const std::vector<Point> &textures, const std::vector<vector<int>> &indices, string objectId);
    bool readMtl(string fname);
    void loadShaders(const std::vector<string>& data);
    void loadShader(const std::vector<string>& data, int index);
    int parseMtl(const string input);
};