#include "Shader.h"
#include <algorithm>

Shader::Shader()
{
    lightDir.setZ(10.0);
    lightDir.setY(1.0);
    _Ns = 1.f;
}
void Shader::setLightDir(Point p)
{
    lightDir = p;
}

void Shader::computeVertIntensities(Triangle &t, Point camera_pos)
{
    // light direction vectors
    Point La, Lb, Lc;
    La = this->lightDir - t.getUntransA();
    Lb = this->lightDir - t.getUntransB();
    Lc = this->lightDir - t.getUntransC();
    La.normalize();
    Lb.normalize();
    Lc.normalize();
    // View direction vecors
    Point Va, Vb, Vc;
    Va = camera_pos - t.getUntransA();
    Vb = camera_pos - t.getUntransB();
    Vc = camera_pos - t.getUntransC();
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

    float LaDotNA = La.dotProduct(t.getNA());
    float LbDotNB = Lb.dotProduct(t.getNB());
    float LcDotNC = Lc.dotProduct(t.getNC());


    if (LaDotNA < 0)
    {
        LaDotNA = 0;
    }
    if (LbDotNB < 0)
    {
        LbDotNB = 0;
    }
    if (LcDotNC < 0)
    {
        LcDotNC = 0;
    }

    float expRaDotVA = pow(Ra.dotProduct(Va), _Ns);
    float expRbDotVB = pow(Rb.dotProduct(Vb), _Ns);
    float expRcDotVC = pow(Rc.dotProduct(Vc), _Ns);

    if (expRaDotVA < 0)
    {
        expRaDotVA = 0;
    }
    if (expRbDotVB < 0)
    {
        expRbDotVB = 0;
    }
    if (expRcDotVC < 0)
    {
        expRcDotVC = 0;
    }

    // Compute lighting by looping over RGB
    for (int i = 0; i < 3; i++)
    {
        intensityA[i] = Ka[i] + Kd[i] * LaDotNA + Ks[i] * expRaDotVA;
        intensityB[i] = Ka[i] + Kd[i] * LbDotNB + Ks[i] * expRbDotVB;
        intensityC[i] = Ka[i] + Kd[i] * LcDotNC + Ks[i] * expRcDotVC;
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