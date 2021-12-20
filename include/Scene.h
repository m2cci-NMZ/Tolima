#pragma once
#include "../include/Object.h"
#include "../include/Shader.h"
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
    void addObject(const Object& object);
    void addShader(const Shader& shader);
    Object getObject(int i);
    Shader getShader(int i);
    int getNumObjects();
    int getNumShaders();
    bool isObjectId(string name);
    Object getObjectById(string name);
    Shader getShaderById(string name);
    void copyShaders(Scene& scene);
};