#pragma once
#include "../Light.h"
class AreaLight : public Light {
    double r;
    Vec3 N;
public:
    AreaLight(Vec3 _pos, Vec3 _color, Vec3 _N, double _r) : Light(_pos, _color), N(_N), r(_r) {}
    virtual Vec3 randomPoint()const {
        double rr = rand01() * r;
        double phi = 2 * PI * rand01();
        Vec3 u = Vec3(1, 0, 0) * N;
        if(u.L2() < EPS) u = Vec3(0, 1, 0) * N;
        return pos + u.rotated(N, phi) * rr;
    }
    ~AreaLight() {};
};

