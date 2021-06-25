#include "Point.h"
#include "Triangle.h"
#include "TriMesh.h"

class Shader{
    private:
        Point lightDir;

    public:
        Shader();
        void setLightDir(Point p);
        TriMesh computeShader(TriMesh object);
};