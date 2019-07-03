#pragma once
#include "../Object.h"
#include "../BoundingBox.h"
#include <algorithm>
#include <Eigen/Dense>
class Bezier3 : public Object {
    Vec3 points[16];

    static Vec3 Point(Vec3 *P, double t) {
        return P[0] * (1 - t) * (1 - t) * (1 - t) + P[1] * 3 * (1 - t) * (1 - t) * t + P[2] * 3 * (1 - t) * t * t + P[3] * t * t * t;
    }
    static Vec3 Tangent(Vec3 *P, double t) {
        return P[0] * (-3) * (1 - t) * (1 - t) + P[1] * (3 * (1 - t) * (1 - t) - 6 * t * (1 - t)) + P[2] * (6 * t * (1 - t) - 3 * t * t) + P[3] * 3 * t * t;
    }
    static Vec3 Point2D(Vec3 *P, double u, double v) {
        Vec3 curve[4];
        for (int i = 0; i < 4; i++) curve[i] = Point(P + 4 * i, u);
        return Point(curve, v);
    }
    static Vec3 Tangent2DU(Vec3 *P, double u, double v) {
        Vec3 Pv[4];
        Vec3 curve[4];
        for (int i = 0; i < 4; i++) {
            Pv[0] = P[i];
            Pv[1] = P[i + 4];
            Pv[2] = P[i + 8];
            Pv[3] = P[i + 12];
            curve[i] = Point(Pv, v);
        }
        return Tangent(curve, u);
    }
    static Vec3 Tangent2DV(Vec3 *P, double u, double v) {
        Vec3 curve[4];
        for (int i = 0; i < 4; i++) curve[i] = Point(P + 4 * i, u);
        return Tangent(curve, v);
    }
    static void Quarter(Vec3 *P, Vec3 *Ps) {
        Vec3 U[16], D[16];
        Vec3 UL[16], UR[16], DL[16], DR[16];
        for (int i = 0; i < 4; i++)    {
            Vec3 P0 = P[i], P1 = P[i + 4], P2 = P[i + 8], P3 = P[i + 12];
            U[i] = P0;
            U[i + 4] = P0 / 2 + P1 / 2;
            U[i + 8] = P0 / 4 + P1 / 2 + P2 / 4;
            U[i + 12] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            D[i] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            D[i + 4] = P1 / 4 + P2 / 2 + P3 / 4;
            D[i + 8] = P2 / 2 + P3 / 2;
            D[i + 12] = P3;
        }
        for (int i = 0; i < 4; i++) {
            Vec3 P0 = U[4 * i], P1 = U[4 * i + 1], P2 = U[4 * i + 2], P3 = U[4 * i + 3];
            UL[4 * i] = P0;
            UL[4 * i + 1] = P0 / 2 + P1 / 2;
            UL[4 * i + 2] = P0 / 4 + P1 / 2 + P2 / 4;
            UL[4 * i + 3] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            UR[4 * i] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            UR[4 * i + 1] = P1 / 4 + P2 / 2 + P3 / 4;
            UR[4 * i + 2] = P2 / 2 + P3 / 2;
            UR[4 * i + 3] = P3;
        }
        for (int i = 0; i < 4; i++) {
            Vec3 P0 = D[4 * i], P1 = D[4 * i + 1], P2 = D[4 * i + 2], P3 = D[4 * i + 3];
            DL[4 * i] = P0;
            DL[4 * i + 1] = P0 / 2 + P1 / 2;
            DL[4 * i + 2] = P0 / 4 + P1 / 2 + P2 / 4;
            DL[4 * i + 3] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            DR[4 * i] = P0 / 8 + P1 * 3.0 / 8 + P2 * 3.0 / 8 + P3 / 8;
            DR[4 * i + 1] = P1 / 4 + P2 / 2 + P3 / 4;
            DR[4 * i + 2] = P2 / 2 + P3 / 2;
            DR[4 * i + 3] = P3;
        }
        for (int i = 0; i < 16; i++) {
            Ps[i] = UL[i];
            Ps[i + 16] = UR[i];
            Ps[i + 32] = DL[i];
            Ps[i + 48] = DR[i];
        }
    } 
    struct Element {
        Vec3 P[16];
        BoundingBox bbox;
        double Uk, Ub, Vk, Vb;
        Element(Vec3 *_P, double _Uk, double _Ub, double _Vk, double _Vb):
            Uk(_Uk), Ub(_Ub), Vk(_Vk), Vb(_Vb) {
            for(int i = 0; i < 16; i++)
                P[i] = _P[i];
            for(int i=0;i<16;i++){
                bbox.lo = min(bbox.lo,P[i]);
                bbox.hi = max(bbox.hi,P[i]);
            }
        }
    };
    static Vec3 Newton(const Ray &ray, Element &e, int MAX_ITER, int &iter) {
        Vec3 x(0, 0, 0), prex(-1, -1, -1);
        while((x - prex).Linf() > EPS && iter < MAX_ITER) {
            prex = x;
            Vec3 L = ray.pos + ray.dir * prex.x;
            Eigen::Vector3d L_(L.x, L.y, L.z);
            Vec3 p = Point2D(e.P, prex.y, prex.z);
            Eigen::Vector3d P_(p.x, p.y, p.z);
            Vec3 tangU = Tangent2DU(e.P, prex[1], prex[2]);
            Vec3 tangV = Tangent2DV(e.P, prex[1], prex[2]);
            Eigen::Matrix3d Jacobi;
            Jacobi(0) = ray.dir.x, Jacobi(1) = ray.dir.y, Jacobi(2) = ray.dir.z;
            Jacobi(3) = -tangU.x, Jacobi(4) = -tangU.y, Jacobi(5) = -tangU.z;
            Jacobi(6) = -tangV.x, Jacobi(7) = -tangV.y, Jacobi(8) = -tangV.z;

            Eigen::Vector3d tmp = Jacobi.inverse() * (L_ - P_);
            x = prex - Vec3(tmp(0), tmp(1), tmp(2));
            iter++;
        }
        return x;
    }

public:
    Bezier3( Vec3 _color,Vec3 *_P) : Object(_color) {
        for(int i = 0; i < 16; i++)
            points[i] = _P[i];
    }
    virtual Intersection intersect(const Ray &_ray);
    ~Bezier3() {};
};
