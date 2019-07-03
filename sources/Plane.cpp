#include "../headers/Objects/Plane.h"

Intersection Plane::intersect(const Ray &_ray) {

    Ray ray = localRay(_ray);
    Intersection intersection;

    double bsinA = - (ray.pos ^ N);
    double sinB = ray.dir ^ N;
    if (fabs(sinB) < EPS) return globalIntersection(intersection);

    double a = bsinA / sinB;
    if (a > EPS && a < INF) {
        Vec3 interpos = ray.pos + ray.dir * a;
        double xx = interpos ^ X;
        double yy = interpos ^ Y;
        if(-EPS <= xx && xx <= x && -EPS <= yy && yy <= y) {
            intersection.N = N;
            intersection.objColor = texture ? texture->at(xx / x, yy / y) : color;
            intersection.pos = interpos;
            intersection.type = bsinA > 0 ? OUT : IN;
        }
    }
    return globalIntersection(intersection);
}