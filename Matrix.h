#pragma once
#include "Point.h"


class Matrix{
private:
    float matrixArray[4][4] ;
public:
    Matrix();
    Matrix(float[4][4]);
    void setValue(int row, int col, float value);
    float getValue(int row, int col);
    void multiplyVector(Point &in);
    void multiplyMatrices(Matrix in, Matrix *out);
    void invertRotMatrix(Matrix m);
    void makeUnit();
    //~Matrix();
};