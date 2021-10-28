#include "Shader.h"

Shader::Shader()
{
    lightDir.setZ(10.0);
    lightDir.setY(1.0);
}
void Shader::setLightDir(Point p)
{
    lightDir = p;
}

void Shader::computeVertIntensities(Triangle &t, Point camera_pos)
{
    // light direction vectors
    Point La, Lb, Lc;
    La = this->lightDir - t.getA();
    Lb = this->lightDir - t.getB();
    Lc = this->lightDir - t.getC();
    La.normalize();
    Lb.normalize();
    Lc.normalize();
    // View direction vecors
    Point Va, Vb, Vc;
    Va = camera_pos - t.getA();
    Vb = camera_pos - t.getB();
    Vc = camera_pos - t.getC();
    Va.normalize();
    Vb.normalize();
    Vc.normalize();
    // Reflection direction vector
    Point Ra, Rb, Rc;
    Ra = t.getNA() * (2 * (La.dotProduct(t.getNA()))) - La;
    Rb = t.getNB() * (2 * (Lb.dotProduct(t.getNB()))) - Lb;
    Rc = t.getNC() * (2 * (Lc.dotProduct(t.getNC()))) - Lc;
    Ra.normalize();
    Rb.normalize();
    Rc.normalize();

    // Compute lighting by looping over RGB
    for (int i = 0; i < 3; i++)
    {
        intensityA[i]=Ka[i] + max(Kd[i]*(La.dotProduct(t.getNA())),0)+ max(Ks[i]*pow(Ra.dotProduct(Va),_Ns),0);
        intensityB[i]=Ka[i] + max(Kd[i]*(Lb.dotProduct(t.getNB())),0)+ max(Ks[i]*pow(Rb.dotProduct(Vb),_Ns),0);
        intensityC[i]=Ka[i] + max(Kd[i]*(Lc.dotProduct(t.getNC())),0)+ max(Ks[i]*pow(Rc.dotProduct(Vc),_Ns),0);
    }
}

float Shader::getIntensityAR()
{
    return intensityA[0];
}
float Shader::getIntensityAG()
{
    return intensityA[1];
}
float Shader::getIntensityAB()
{
    return intensityA[2];
}

float Shader::getIntensityBR()
{
    return intensityB[0];
}
float Shader::getIntensityBG()
{
    return intensityB[1];
}
float Shader::getIntensityBB()
{
    return intensityB[2];
}

float Shader::getIntensityCR()
{
    return intensityC[0];
}
float Shader::getIntensityCG()
{
    return intensityC[1];
}
float Shader::getIntensityCB()
{
    return intensityC[2];
}

void Shader::setKa(float r, float g, float b)
{
    this->Ka[0] = r;
    this->Ka[1] = g;
    this->Ka[2] = b;
}

void Shader::setKd(float r, float g, float b)
{
    this->Kd[0] = r;
    this->Kd[1] = g;
    this->Kd[2] = b;
}

void Shader::setKs(float r, float g, float b)
{
    this->Ks[0] = r;
    this->Ks[1] = g;
    this->Ks[2] = b;
}