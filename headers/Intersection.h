#pragma once
#include "Vec3.h"
enum IntersectionType {
    IN, OUT, MISS
};
class Object;
struct Intersection {
    Object *obj = nullptr;
    Vec3 pos, N, objColor;
    IntersectionType type = MISS;
};
