#include "Point.h"
#include "TriMesh.h"
#include "Matrix.h"

class Camera
{
private:
    Point position;
    Point target;
    Point up;
    float yaw;
    float pitch;
    Matrix pointAt;

public:
    Camera();
    void makePointAt();
    void turn(float _yaw);
    void moveForward(float speed);
    void moveBackward(float speed);
    void moveUp(float speed);
    void moveDown(float speed);
    Matrix makerotationZMatrix(float rot);
    Matrix makerotationYMatrix(float rot);
    Matrix makeFPSTransform(Point position, float pitch, float yaw);

    void transform(Point in, Point* out);
    Point getPosition();
    TriMesh worldTransform(TriMesh in);
    TriMesh ndcTransform(TriMesh in, int height, int width);
    TriMesh viewPortTransform(TriMesh in, int height, int width);

};