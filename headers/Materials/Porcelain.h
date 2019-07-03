#pragma once
#include "../Material.h"
class Porcelain : public Material {
public:
    Porcelain() : Material(0.8, 0.2, 0.0, 2, 0) {}
    ~Porcelain() {};
};
