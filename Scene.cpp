#include "Scene.h"

Scene::Scene()
{
}
void Scene::addObject(Object object)
{
    objects.push_back(object);
    numObjects++;
}
Object Scene::getObject(int i)
{
    return objects[i];
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
        else
        {
            return false;
        }
    }
}