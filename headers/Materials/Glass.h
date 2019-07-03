#pragma once
#include "../Material.h"
class Glass : public Material {
public:
    Glass() : Material(0.2, 0.1, 0.70, 20, 1.52) {}
    ~Glass() {};
};

