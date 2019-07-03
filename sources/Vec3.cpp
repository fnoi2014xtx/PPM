#include "../headers/Vec3.h"
#include <cstdlib>
Vec3 Vec3::rotated(const Vec3 &axis, const double &angle)const {
    if (fabs(angle) < EPS)
        return Vec3(x, y, z);
    Vec3 ret;
    double s = sin(angle), c = cos(angle);
    ret.x += x * (axis.x * axis.x + (1 - axis.x * axis.x) * c);
    ret.x += y * (axis.x * axis.y * (1 - c) - axis.z * s);
    ret.x += z * (axis.x * axis.z * (1 - c) + axis.y * s);
    ret.y += x * (axis.y * axis.x * (1 - c) + axis.z * s);
    ret.y += y * (axis.y * axis.y + (1 - axis.y * axis.y) * c);
    ret.y += z * (axis.y * axis.z * (1 - c) - axis.x * s);
    ret.z += x * (axis.z * axis.x * (1 - c) - axis.y * s);
    ret.z += y * (axis.z * axis.y * (1 - c) + axis.x * s);
    ret.z += z * (axis.z * axis.z + (1 - axis.z * axis.z) * c);
    return ret.normalized();
}
Vec3 Vec3::reflected(const Vec3 &N)const {
    return (*this - 2 * N * (*this ^ N)).normalized();
}
Vec3 Vec3::refracted(const Vec3 &N, const double &n)const {

    double cosI = -(*this ^ N);
    double cosR2 = 1 - (1 - pow(cosI, 2)) * pow(n, 2);
    return (cosR2 > EPS) ? // 是否全反射？
           ((*this * n) + N * ( n * cosI - sqrt( cosR2 ) )).normalized()
           : this->reflected(N);
    //如果发生全反射，这部分颜色的贡献就由反射出
}
