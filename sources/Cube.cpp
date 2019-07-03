#include "../headers/Objects/Cube.h"
Intersection Cube::intersect(const Ray &_ray) {
    Ray ray = localRay(_ray);
    Intersection intersection;

    Vec3 poss[6] = {Vec3(d / 2, d / 2, d / 2), Vec3(d / 2, d / 2, d / 2), Vec3(d / 2, d / 2, d / 2), Vec3(-d / 2, -d / 2, -d / 2), Vec3(-d / 2, -d / 2, -d / 2), Vec3(-d / 2, -d / 2, -d / 2)};
    Vec3 Xs[6] = {Vec3(0, -1, 0), Vec3(0, 0, -1), Vec3(-1, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), Vec3(1, 0, 0)};
    Vec3 Ys[6] = {Vec3(0, 0, -1), Vec3(-1, 0, 0), Vec3(0, -1, 0), Vec3(0, 1, 0), Vec3(1, 0, 0), Vec3(0, 0, 1)};
    double x = d, y = d;
    double minl = INF;
    for(int i = 0; i < 6; i++) {
        Vec3 N = (Xs[i] * Ys[i]).normalized();
        double bsinA = - ((ray.pos - poss[i]) ^ N);
        double sinB = ray.dir ^ N;
        if (fabs(sinB) < EPS) continue;

        double a = bsinA / sinB;
        if (a > EPS && a < minl) {
            Vec3 interpos = (ray.pos - poss[i]) + ray.dir * a;
            double xx = interpos ^ Xs[i];
            double yy = interpos ^ Ys[i];
            if(-EPS <= xx && xx <= x && -EPS <= yy && yy <= y) {
                minl = a;
                intersection.N = N;
                intersection.objColor = color;
                intersection.pos = interpos + poss[i];
                intersection.type = bsinA > 0 ? OUT : IN;
            }
        }
    }
    return globalIntersection(intersection);
}
