#pragma once
#include "../Material.h"
class Mirror : public Material {
public:
    Mirror() : Material(0, 1, 0, 2, 0) {}
    ~Mirror() {};
};
