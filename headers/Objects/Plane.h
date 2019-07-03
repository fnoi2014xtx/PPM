#pragma once
#include "../Object.h"
class Plane : public Object {
    Vec3 N;
    Vec3 X, Y;
    double x, y;
public:
    Plane(Vec3 _color, Vec3 _X, Vec3 _Y, double _x, double _y) : Object(_color), X(_X), Y(_Y), x(_x), y(_y) {
        N = X * Y;
    }
    virtual Intersection intersect(const Ray &_ray);
    ~Plane() {};
};
