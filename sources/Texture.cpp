#include "../headers/Texture.h"
#include <cmath>
Vec3 Texture::at(double x, double y) {
    double row = (x - floor(x)) * height, col = (y - floor(y)) * width;
    int r1 = (int)floor(row + EPS), r2 = r1 + 1;
    int c1 = (int)floor(col + EPS), c2 = c1 + 1;
    double detR = r2 - row, detC = c2 - col;
    r1 = (r1 >= 0) ? (r1 >= height ? 0 : r1) : (height - 1);
    c1 = (c1 >= 0) ? (c1 >= width ? 0 : c1) : (width - 1);
    r2 = (r2 < height) ? r2 : 0;
    c2 = (c2 < width) ? c2 : 0;
    return img[r1][c1] * detR * detC
           + img[r1][c2] * detR * (1 - detC)
           + img[r2][c1] * (1 - detR) * detC
           + img[r2][c2] * (1 - detR) * (1 - detC);
}