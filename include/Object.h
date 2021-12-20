#pragma once
#include "../include/TriMesh.h"
#include "../include/Shader.h"
#include <vector>

class Object : public TriMesh
{
private:
    string _shaderid;
    string _id;

public:
    Object();
    Object(string id, string shared_id);

    void setShaderId(const string &shaderid);
    void setId(const string &id);

    string getShaderId();
    string getId();
};