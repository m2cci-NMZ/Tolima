#pragma once
#include "TriMesh.h"
#include "Shader.h"
#include <vector>

class Object{
    private:
        TriMesh _mesh;
        Shader _shader;
    public:
        Object(TriMesh mesh, Shader shader);
        void setMesh(const TriMesh& mesh);
        void setShader(const Shader& shader);

        TriMesh getMesh();
        Shader getShader();
}