#pragma once
#include "TriMesh.h"
#include "Shader.h"
#include <vector>

class Object{
    private:
        TriMesh _mesh;
        Shader _shader;
        string _id;
    public:
        Object(TriMesh mesh, Shader shader, string id);

        void setMesh(const TriMesh& mesh);
        void setShader(const Shader& shader);
        void setId(const string& id);

        TriMesh getMesh();
        Shader getShader();
        string getId();
}