#pragma once
#include <cmath>
#include "Parameters.h"
#include <iostream>
#include <algorithm>
#include <cstdlib>
using std::cout;
using std::endl;
#define rand01() (1.0*rand() / double(RAND_MAX))
#define PI acos(-1)
class Vec3 {
public:
    double x, y, z;
    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(const double &_x, const double &_y, const double &_z): x(_x), y(_y), z(_z) {}
    double &operator[] (const int &index) {
        return index == 0 ? x : (index == 1 ? y : z);
    }
    double at(const int &index)const {
        return index == 0 ? x : (index == 1 ? y : z);
    }
    bool operator==(const Vec3 &that)const {
        return (fabs(x - that.x) < EPS) && (fabs(y - that.y) < EPS) && (fabs(z - that.z) < EPS);
    }
    bool operator!=(const Vec3 &that)const {
        return (fabs(x - that.x) >= EPS) || (fabs(y - that.y) >= EPS) || (fabs(z - that.z) >= EPS);
    }
    Vec3 operator*(const Vec3 &that)const {
        return Vec3(y * that.z - z * that.y, z * that.x - x * that.z, x * that.y - y * that.x);
    }
    Vec3 operator-(const Vec3 &that)const {
        return Vec3(x - that.x, y - that.y, z - that.z);
    }
    Vec3 operator+(const Vec3 &that)const {
        return Vec3(x + that.x, y + that.y, z + that.z);
    }
    double operator^(const Vec3 &that)const {
        return x * that.x + y * that.y + z * that.z;
    }
    Vec3 operator/(const double &k)const {
        return Vec3(x / k, y / k, z / k);
    }
    Vec3 operator*(const double &k)const {
        return Vec3(x * k, y * k, z * k);
    }
    Vec3 operator - ()const {
        return Vec3(-x, -y, -z);
    }
    Vec3 operator + ()const {
        return Vec3(+x, +y, +z);
    }
    bool operator <=(const Vec3 &that)const {
        return x <= that.x + EPS && y <= that.y + EPS && z <= that.z + EPS;
    }
    bool operator >=(const Vec3 &that)const {
        return x >= that.x - EPS && y >= that.y - EPS && z >= that.z - EPS;
    }
    bool nonzero()const {
        return fabs(x) > EPS || fabs(y) > EPS || fabs(z) > EPS;
    }
    double L0()const {
        return int(fabs(x) > EPS) + int(fabs(y) > EPS) + int(fabs(z) > EPS);
    }
    double L1()const {
        return fabs(x) + fabs(y) + fabs(z);
    }
    double L2()const {
        return sqrt(x * x + y * y + z * z);
    }
    double L2sqr()const {
        return x * x + y * y + z * z;
    }
    double Linf()const {
        return std::max(std::max(fabs(x), fabs(y)), fabs(z));
    }
    void normalize() {
        double l = L2();
        if(l <= EPS) {
            x = y = z = 0.0;
        } else {
            x /= l;
            y /= l;
            z /= l;
        }
    }
    Vec3 normalized()const {
        double l = L2();
        if(l < EPS)
            return Vec3(0, 0, 0);
        else
            return Vec3(x / l, y / l, z / l);
    }
    friend Vec3 operator * (const double &k, const Vec3 &that) {
        return Vec3(that.x * k, that.y * k, that.z * k);
    }
    friend Vec3 max(const Vec3 &a, const Vec3 &b) {
        Vec3 ret;
        ret.x = a.x > b.x ? a.x : b.x;
        ret.y = a.y > b.y ? a.y : b.y;
        ret.z = a.z > b.z ? a.z : b.z;
        return ret;
    }
    friend Vec3 min(const Vec3 &a, const Vec3 &b) {
        Vec3 ret;
        ret.x = a.x > b.x ? b.x : a.x;
        ret.y = a.y > b.y ? b.y : a.y;
        ret.z = a.z > b.z ? b.z : a.z;
        return ret;
    }
    friend std::ostream &operator << ( std::ostream &os, const Vec3 &v ) {
        os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
        return os;
    }
    Vec3 operator & (const Vec3 &that)const {
        return Vec3(x * that.x, y * that.y, z * that.z);
    }
    void operator += (const Vec3 &that) {
        x += that.x;
        y += that.y;
        z += that.z;
    }
    void operator -= (const Vec3 &that) {
        x -= that.x;
        y -= that.y;
        z -= that.z;
    }
    void operator *= (const double &k) {
        x *= k;
        y *= k;
        z *= k;
    }
    void operator /= (const double &k) {
        x /= k;
        y /= k;
        z /= k;
    }
    void operator &=(const Vec3 &that) {
        x *= that.x;
        y *= that.y;
        z *= that.z;
    }

    static Vec3 Random() {  //球面上均匀采样
        double x, y, z;
        do {
            x = 2 * rand01() - 1;
            y = 2 * rand01() - 1;
            z = 2 * rand01() - 1;
        } while ( x * x + y * y + z * z < EPS || x * x + y * y + z * z > 1);
        return Vec3( x, y, z ).normalized();
    }
    static Vec3 RandomCos(const Vec3 &N) {  //反射表面根据与法向量夹角的cos值进行加权采样
        double theta = asin(rand01());      //随机采样的向量与法向量之间的夹角
        double phi = 2 * PI * rand01();     //任取与法向量垂直的一个向量u偏离theta的旋转轴，然后再绕N旋转phi
        Vec3 u = Vec3(1, 0, 0) * N;
        if(u.L2() < EPS)u = Vec3(0, 1, 0) * N;
        u.normalize();
        Vec3 ret = N.rotated(u, theta);
        return ret.rotated(N, phi);
    }
    //三种操作均返回单位向量
    Vec3 reflected(const Vec3 &N)const;
    Vec3 refracted(const Vec3 &N, const double &n)const;
    Vec3 rotated(const Vec3 &axis, const double &angle)const;
};
