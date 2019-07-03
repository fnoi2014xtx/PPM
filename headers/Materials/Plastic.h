#pragma once
#include "../Material.h"
class Plastic : public Material {
public:
    Plastic() : Material(1.0, 0.0, 0.0, 0.1, 0) {}
    ~Plastic() {};
};
