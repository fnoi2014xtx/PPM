#pragma once
#include "../Object.h"
class Cube : public Object {
    double d;
public:
    Cube(Vec3 _color, double _d) : Object(_color), d(_d) {}
    virtual Intersection intersect(const Ray &_ray);
    ~Cube() {}
};

