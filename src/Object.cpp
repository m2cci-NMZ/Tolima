#include "../include/Object.h"

Object::Object()
{
    _id = "";
    _shaderid = "";
}
Object::Object(string id, string shader_id)
{
    _id = id;
    _shaderid = shader_id;
}

void Object::setId(const string &id)
{
    _id = id;
}
string Object::getShaderId()
{
    return _shaderid;
}
string Object::getId()
{
    return _id;
}
void Object::setShaderId(const string &shader_id)
{
    _shaderid = shader_id;
}