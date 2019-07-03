#include "../headers/Objects/Bezier3.h"
#include <queue>
Intersection Bezier3::intersect(const Ray &_ray) {

    Ray ray = localRay(_ray);
    std::queue<Element> queue;
    Vec3 Ps[64];
    queue.push(Element(points, 1, 0, 1, 0));
    while (!queue.empty()) {
        Element x = queue.front();
        if ((x.bbox.hi - x.bbox.lo).Linf() < 1e-2) break;
        queue.pop();
        Quarter(x.P, Ps);
        double Uk = x.Uk, Ub = x.Ub, Vk = x.Vk, Vb = x.Vb;
        Element UL(Ps, Uk / 2, Ub, Vk / 2, Vb);
        if (UL.bbox.intersect(ray))queue.push(UL);
        Element UR(Ps + 16, Uk / 2, Ub + Uk / 2, Vk / 2, Vb);
        if (UR.bbox.intersect(ray))queue.push(UR);
        Element DL(Ps + 32, Uk / 2, Ub, Vk / 2, Vb + Vk / 2);
        if (DL.bbox.intersect(ray))queue.push(DL);
        Element DR(Ps + 48, Uk / 2, Ub + Uk / 2, Vk / 2, Vb + Vk / 2);
        if (DR.bbox.intersect(ray))queue.push(DR);
    }

    double minl = INF, bestDist = -1, bestU = -1, bestV = -1;
    Element bestPatch(points, 1, 0, 1, 0);
    while (!queue.empty()) {
        Element cur = queue.front();
        queue.pop();
        int iter = 0;
        Vec3 x = Newton(ray, cur, PPM_NEWTON_ITER, iter);
        if (iter < PPM_NEWTON_ITER && x[0] > EPS && x[0] < minl && x[1] >= -EPS && x[1] <= 1 + EPS
                && x[2] >= -EPS && x[2] <= 1 + EPS) {
            minl = x[0];
            bestDist = x[0];
            bestU = x[1];
            bestV = x[2];
            bestPatch = cur;
        }
    }
    Intersection intersection;
    // 不相交的情况：距离过近/过远/UV越界
    if (bestDist < EPS  || bestDist > INF
            || bestU < -EPS || bestU > 1 + EPS
            || bestV < -EPS || bestV > 1 + EPS) {
        //cout<<bestU<<" "<<bestV<<endl;
        //cout<<"MISS"<<endl;
        return globalIntersection(intersection);

    }
    intersection.pos = ray.pos + ray.dir * bestDist;
    Vec3 tangU = Tangent2DU(bestPatch.P, bestU, bestV);
    Vec3 tangV = Tangent2DV(bestPatch.P, bestU, bestV);
    intersection.N = (tangU * tangV).normalized();
    if((intersection.N ^ ray.dir) > 0)intersection.N = -intersection.N;
    intersection.type = OUT;
    if(texture)
        intersection.objColor = texture->at(bestPatch.Uk * bestU + bestPatch.Ub, bestPatch.Vk * bestV + bestPatch.Vb);
    else
        intersection.objColor = color;
    return globalIntersection(intersection);
}