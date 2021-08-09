#include "Shader.h"

Shader::Shader()
{
    lightDir.setZ(-1.0);
    lightDir.setY(1.0);
}
void Shader::setLightDir(Point p)
{
    lightDir = p;
}
TriMesh Shader::computeShader(TriMesh object)
{
    TriMesh out;
    lightDir.normalize();
    for (auto tri : object.getTriangles())
    {
        tri.computeNormal();
        float lum = lightDir.dotProduct(tri.getNormal());
        if (lum > 0)
        {
            tri.setLum(lum);
        }
        else
        {
            tri.setLum(0.0);
        }
        out.addTriangle(tri);
    }
    return out;
}
void Shader::computeVertIntensities(Triangle &t)
{
    intensityA = t.getNA().dotProduct(lightDir);
    intensityB = t.getNB().dotProduct(lightDir);
    intensityC = t.getNC().dotProduct(lightDir);
    if (intensityA < 0)
    {
        intensityA = 0;
    }
    if (intensityB < 0)
    {
        intensityB = 0;
    }
    if (intensityC < 0)
    {
        intensityC = 0;
    }
}
float Shader::getIntensityA()
{
    return intensityA;
}
float Shader::getIntensityB()
{
    return intensityB;
}
float Shader::getIntensityC()
{
    return intensityC;
}