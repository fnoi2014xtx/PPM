#pragma once
#include "Vec3.h"
#include <string>
#include "LodePNG/lodepng.h"

namespace PNGImage {
    void read(const std::string &imgName, int &height, int &width, Vec3 ** &img);
    void write(const std::string &imgName, int height, int width, Vec3 **img);
}
