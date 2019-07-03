#pragma once
#include "Vec3.h"
class Light {
protected:
    Vec3 pos;
    Vec3 color;
public:
    Light(Vec3 _pos, Vec3 _color) : pos(_pos), color(_color) {}
    Vec3 Color()const {
        return color;
    }
    virtual Vec3 randomPoint()const = 0;
    ~Light() {};
};

