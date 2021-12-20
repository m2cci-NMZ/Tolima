#include "../include/Matrix.h"

Matrix::Matrix()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrixArray[i][j] = 0;
        }
    }
}
Matrix::Matrix(float _matrix[4][4])
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            matrixArray[i][j] = _matrix[i][j];
        }
    }
}
float Matrix::getValue(int row, int col)
{
    return matrixArray[row][col];
}
void Matrix::setValue(int row, int col, float value)
{
    matrixArray[row][col] = value;
}

void Matrix::multiplyVector(Point &in)
{
    float x = in.getX() * matrixArray[0][0] + in.getY() * matrixArray[1][0] + in.getZ() * matrixArray[2][0] + matrixArray[3][0] * in.getW();

    float y = in.getX() * matrixArray[0][1] + in.getY() * matrixArray[1][1] + in.getZ() * matrixArray[2][1] + matrixArray[3][1] * in.getW();

    float z = in.getX() * matrixArray[0][2] + in.getY() * matrixArray[1][2] + in.getZ() * matrixArray[2][2] + matrixArray[3][2] * in.getW();

    float w = in.getX() * matrixArray[0][3] + in.getY() * matrixArray[1][3] + in.getZ() * matrixArray[2][3] + matrixArray[3][3] * in.getW();

    in.setX(x);
    in.setY(y);
    in.setZ(z);
    in.setW(w);
}

void Matrix::multiplyMatrices(Matrix in, Matrix *out)
{
    float value;
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            value = in.getValue(r, 0) * matrixArray[0][c] + in.getValue(r, 1) * matrixArray[1][c] + in.getValue(r, 2) * matrixArray[2][c] + in.getValue(r, 3) * matrixArray[3][c];
            out->setValue(r, c, value);
        }
    }
}

void Matrix::makeUnit()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
            {
                matrixArray[i][j] = 1;
            }
            else
            {
                matrixArray[i][j] = 0;
            }
        }
    }
}

void Matrix::invertRotMatrix(Matrix m)
{
    //float m[4][4];
    matrixArray[0][0] = m.getValue(0, 0);
    matrixArray[0][1] = m.getValue(1, 0);
    matrixArray[0][2] = m.getValue(2, 0);
    matrixArray[0][3] = 0.0f;
    matrixArray[1][0] = m.getValue(0, 1);
    matrixArray[1][1] = m.getValue(1, 1);
    matrixArray[1][2] = m.getValue(2, 1);
    matrixArray[1][3] = 0.0f;
    matrixArray[2][0] = m.getValue(0, 2);
    matrixArray[2][1] = m.getValue(1, 2);
    matrixArray[2][2] = m.getValue(2, 2);
    matrixArray[2][3] = 0.0f;
    matrixArray[3][0] = -(m.getValue(3, 0) * matrixArray[0][0] + m.getValue(3, 1) * matrixArray[1][0] + m.getValue(3, 2) * matrixArray[2][0]);
    matrixArray[3][1] = -(m.getValue(3, 0) * matrixArray[0][1] + m.getValue(3, 1) * matrixArray[1][1] + m.getValue(3, 2) * matrixArray[2][1]);
    matrixArray[3][2] = -(m.getValue(3, 0) * matrixArray[0][2] + m.getValue(3, 1) * matrixArray[1][2] + m.getValue(3, 2) * matrixArray[2][2]);
    matrixArray[3][3] = 1.0f;
}