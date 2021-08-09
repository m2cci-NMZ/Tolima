#pragma once
#include "Point.h"
#include "Triangle.h"
#include "TriMesh.h"

class Shader{
    private:
        Point lightDir;
        float intensityA;
        float intensityB;
        float intensityC;

    public:
        Shader();
        void setLightDir(Point p);
        TriMesh computeShader(TriMesh object);
        void computeVertIntensities(Triangle& t);
        float getIntensityA();
        float getIntensityB();
        float getIntensityC();
};