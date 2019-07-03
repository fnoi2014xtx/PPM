#pragma once
#include "Vec3.h"
struct Ray {
    Vec3 pos;
    Vec3 dir;
    Ray() {}
    Ray(Vec3 _pos, Vec3 _dir): pos(_pos), dir(_dir) {}
};