#pragma once
#include "../Object.h"
class Sphere : public Object {
    double r;
public:
    Sphere(Vec3 _color, double _r) : Object(_color), r(_r) {}
    virtual Intersection intersect(const Ray &_ray);
    ~Sphere() {}
};

