#include "Shader.h"

Shader::Shader(){
    lightDir.setZ(-1.0);
    lightDir.setY(1.0);
}
void Shader::setLightDir(Point p){
    lightDir = p;
}
TriMesh Shader::computeShader(TriMesh object){
    TriMesh out;
    lightDir.normalize();
    for (auto tri: object.getTriangles()){
            tri.computeNormal();
            float lum = lightDir.dotProduct(tri.getNormal());
            tri.setLum(lum);
            out.addTriangle(tri);
    }
    return out;
}