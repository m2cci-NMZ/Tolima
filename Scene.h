#pragma once
#include "Object.h"
#include "Shader.h"
#include <vector>

class Scene
{
private:
    vector<Object> objects;
    vector<Shader> shaders;
    int numObjects = 0;
    int numShaders = 0;

public:
    Scene();
    void addObject(Object object);
    void addShader(Shader shader);
    Object getObject(int i);
    Shader getShader(int i);
    int getNumObjects();
    int getNumShaders();
    bool isObjectId(string name);
    Object getObjectById(string name);
    Shader getShaderById(string name);
};