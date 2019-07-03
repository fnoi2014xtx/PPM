#pragma once
#include "Vec3.h"
#include <string>
#include "utils.h"
class Texture {
    Vec3 **img = nullptr;
    int height, width;
public:
    Texture(const std::string &name) {
        PNGImage::read(name, height, width, img);
    }
    Vec3 at(double x, double y);
    ~Texture() {
        if (img != nullptr) {
            for (int i = 0; i < height; i++)
                delete[] img[i];
            delete[] img;
        }
    }
};

