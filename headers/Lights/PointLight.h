#pragma once
#include "../Light.h"
class PointLight : public Light {
public:
    PointLight(Vec3 _pos, Vec3 _color) : Light(_pos, _color) {}
    virtual Vec3 randomPoint()const {
        return pos;
    }
    ~PointLight() {};
};

