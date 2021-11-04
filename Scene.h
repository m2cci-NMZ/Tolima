#pragma once
#include "Object.h"
#include <vector>

class Scene
{
private:
    vector<Object> objects;
    int numObjects = 0;

public:
    Scene();
    void addObject(Object object);
    Object getObject(int i);
    int getNumObjects();
    bool isObjectId(string name);
}