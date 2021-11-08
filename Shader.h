#pragma once
#include "Point.h"
#include "Triangle.h"
#include "TriMesh.h"
#include <tgmath.h> 
#include <algorithm>

class Shader{
    private:
        Point lightDir;
        float intensityA[3];
        float intensityB[3];
        float intensityC[3];
        float _Ns; 
        float Ka[3] = {0.25f, 0.25f, 0.25f};
        float Kd[3] = {0.4f, 0.4f, 0.4f};
        float Ks[3] = {0.35f, 0.35f, 0.35f};
        string _id;



    public:
        Shader();
        void setLightDir(Point p);
        void computeVertIntensities(Triangle& t, Point camera_pos);
        float getIntensityAR();
        float getIntensityAG();
        float getIntensityAB();
        float getIntensityBR();
        float getIntensityBG();
        float getIntensityBB();
        float getIntensityCR();
        float getIntensityCG();
        float getIntensityCB();
        void setKa(float r, float g, float b);
        void setKd(float r, float g, float b);
        void setKs(float r, float g, float b);
        void setNs(float Ns);
        void setId(const string& id);
        string getId();
};