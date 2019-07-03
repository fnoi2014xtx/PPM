#pragma once
#include "Vec3.h"
#include <algorithm>
struct BoundingBox {
    Vec3 lo, hi;
    BoundingBox() {
        lo = Vec3(INF, INF, INF);
        hi = Vec3(-INF, -INF, -INF);
    }
    bool intersect(const Ray &ray)const {
        double ox = ray.pos.x, oy = ray.pos.y, oz = ray.pos.z;
        double dx = ray.dir.x, dy = ray.dir.y, dz = ray.dir.z;
        double tx_min, ty_min, tz_min, tx_max, ty_max, tz_max;
        //lo.x,lo.y,lo.z为包围体的最小顶点
        //hi.x,hi.y,hi.z为包围体的最大顶点
        if(fabs(dx) < EPS) {
            //若射线方向矢量的x轴分量为0且原点不在盒体内
            if(ox < hi.x || ox > lo.x)
                return false ;
        } else {
            if(dx >= 0) {
                tx_min = (lo.x - ox) / dx;
                tx_max = (hi.x - ox) / dx;
            } else {
                tx_min = (hi.x - ox) / dx;
                tx_max = (lo.x - ox) / dx;
            }

        }
        if(fabs(dy) < EPS) {
            //若射线方向矢量的x轴分量为0且原点不在盒体内
            if(oy < hi.y || oy > lo.y)
                return false ;
        } else {
            if(dy >= 0) {
                ty_min = (lo.y - oy) / dy;
                ty_max = (hi.y - oy) / dy;
            } else {
                ty_min = (hi.y - oy) / dy;
                ty_max = (lo.y - oy) / dy;
            }

        }


        if(fabs(dz) < EPS) {
            //若射线方向矢量的x轴分量为0且原点不在盒体内
            if(oz < hi.z || oz > lo.z)
                return false ;
        } else {
            if(dz >= 0) {
                tz_min = (lo.z - oz) / dz;
                tz_max = (hi.z - oz) / dz;
            } else {
                tz_min = (hi.z - oz) / dz;
                tz_max = (lo.z - oz) / dz;
            }

        }

        double t0, t1;

        //光线进入平面处（最靠近的平面）的最大t值
        t0 = std::max(tz_min, std::max(tx_min, ty_min));

        //光线离开平面处（最远离的平面）的最小t值
        t1 = std::min(tz_max, std::min(tx_max, ty_max));

        return t0 < t1;
    }
};