#include "Object.h"

Object::Object(TriMesh mesh, Shader shader)
{
    _mesh = mesh;
    _shader = shader;
}
void Object::setMesh(const TriMesh& mesh){
    _mesh = mesh;
}
void Object::setShader(const Shader& shader){
    _shader =shader;
}
Shader Object::getShader(){
    return _shader;
}
TriMesh Object::getMesh(){
    return _mesh;
}