#include "Scene.h"

Scene::Scene()
{
}
void Scene::addObject(Object object)
{
    objects.push_back(object);
    numObjects++;
}
void Scene::addShader(Shader shader)
{
    shaders.push_back(shader);
    numShaders++;
}
Object Scene::getObject(int i)
{
    return objects[i];
}
Shader Scene::getShader(int i)
{
    return shaders[i];
}
int Scene::getNumObjects()
{
    return numObjects;
}
bool Scene::isObjectId(string name)
{
    for (int i = 0; i < numObjects; i++)
    {
        if (objects[i].getId() == name)
        {
            return true;
        }
    }
    return false;
}
Object Scene::getObjectById(string s)
{
    for (int i = 0; i < numObjects; i++)
    {
        if (this->getObject(i).getId() == s)
        {
            return this->getObject(i);
        }
    }
}
Shader Scene::getShaderById(string s)
{
    for (int i = 0; i < numShaders; i++)
    {
        if (this->getShader(i).getId() == s)
        {
            return this->getShader(i);
        }
    }
}
void Scene::copyShaders(Scene& scene){
    for (int i = 0; i<scene.numShaders; i++){
        this->addShader(scene.getShader(i));
    }
}