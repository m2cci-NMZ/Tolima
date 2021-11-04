#include "Object.h"

Object::Object(TriMesh mesh, Shader shader, string id)
{
    _mesh = mesh;
    _shader = shader;
    _id = id;
}
void Object::setMesh(const TriMesh &mesh)
{
    _mesh = mesh;
}
void Object::setId(const string &id)
{
    _id = id;
}
void Object::setShader(const Shader &shader)
{
    _shader = shader;
}
Shader Object::getShader()
{
    return _shader;
}
string Object::getId()
{
    return _id;
}
TriMesh Object::getMesh()
{
    return _mesh;
}