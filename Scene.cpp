#include "Scene.h"

Scene::Scene(){

}
void Scene::addObject(Object object){
    objects.push_back(object);
    numObjects++;
}
Object Scene::getObject(int i){
    return objects[i];
}
int Scene::getNumObjects(){
    return numObjects;
}