#include "../headers/Objects/Sphere.h"
#include <cmath>
#include <algorithm>
Intersection Sphere::intersect(const Ray &_ray) {
    Ray ray = localRay(_ray);
    Intersection intersection;
    Vec3 L =  - ray.pos;
    double tangent = L ^ ray.dir;
    double det2 = r * r - (L.L2sqr() - tangent * tangent);
    if (det2 < EPS) return globalIntersection(intersection);

    double det = sqrt(det2);
    double dist1 = tangent - det, dist2 = tangent + det;
    if (dist1 > INF || (dist1 < EPS && dist2 > INF)) return globalIntersection(intersection);
    if (dist2 < EPS) return globalIntersection(intersection);
    double dis = (dist1 > EPS) ? dist1 : dist2;
    intersection.pos = ray.pos + ray.dir * dis;
    intersection.N = intersection.pos.normalized();
    intersection.objColor = color;
    if(dist1 > EPS)intersection.type = OUT;
    else intersection.type = IN;
    return globalIntersection(intersection);
}
