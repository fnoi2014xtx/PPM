#pragma once
#include "../Material.h"
class Metal : public Material {
public:
    Metal() : Material(0.97, 0.03, 0, 60, 0) {}
    ~Metal() {};
};

