#pragma once
#include "../Object.h"
#include "../BoundingBox.h"
#include <string>
class TriangleMesh : public Object {
    Vec3 vertexs[TRIANGLEMESH_MAXN];
    struct Triangle {
        Vec3 A, B, C;
        double Helen(double a, double b, double c)const {
            double p = (a + b + c) / 2;
            return sqrt(p * (p - a) * (p - b) * (p - c));
        }
        Intersection intersect(const Ray &ray, const Vec3 &color)const {
            Intersection intersection;
            Vec3 N = ((B - A) * (C - A)).normalized();
            double bsinA = - ((ray.pos - A) ^ N);
            double sinB = ray.dir ^ N;
            if (fabs(sinB) < EPS) return intersection;

            double a = bsinA / sinB;
            if (a > EPS && a < INF) {
                Vec3 interpos = ray.pos + ray.dir * a;
                double xx = (B - A).L2(), yy = (C - A).L2(), zz = (B - C).L2();
                double S = Helen(xx, yy, zz);
                double x = (interpos - A).L2(), y = (interpos - B).L2(), z = (interpos - C).L2();
                if(fabs(Helen(x, y, xx) + Helen(y, z, zz) + Helen(x, z, yy) - S) < EPS) {
                    intersection.N = N;
                    intersection.objColor = color;
                    intersection.pos = interpos;
                    intersection.type = bsinA > 0 ? OUT : IN;
                }
            }
            return intersection;
        }
    } facets[TRIANGLEMESH_MAXM];

    int n = 0, m = 0;

    struct AABBTree {
        struct Node {
            Triangle facet;
            BoundingBox bbox;
            Node *l = nullptr, *r = nullptr;
            void update() {
                //cout<<"begin"<<endl;
                bbox.lo = min(min(facet.A, facet.B), facet.C);
                bbox.hi = max(max(facet.A, facet.B), facet.C);
                if(l != nullptr) {
                    bbox.lo = min(bbox.lo, l->bbox.lo);
                    bbox.hi = max(bbox.hi, l->bbox.hi);
                }
                if(r != nullptr) {
                    bbox.lo = min(bbox.lo, r->bbox.lo);
                    bbox.hi = max(bbox.hi, r->bbox.hi);
                }
                //cout<<"end"<<endl;
            }
        } pond[TRIANGLEMESH_MAXN], *root = nullptr;
        int pondcnt = 0;
        Vec3 color;
        Node *newnode(const Triangle &facet) {
            Node *nd = &pond[pondcnt++];
            nd->l = nd->r = nullptr;
            nd->facet = facet;
            return nd;
        }
        Node *build(int l, int r, Triangle *fs) {
            //cout<<l<<" "<<r<<endl;
            int mid = (l + r) >> 1;
            Node *rt = newnode(fs[mid]);
            if(l < mid)rt->l = build(l, mid - 1, fs);
            if(mid < r)rt->r = build(mid + 1, r, fs);
            rt->update();
            return rt;
        }
        Intersection find(Node *rt, const Ray &ray)const {
            if(!rt->bbox.intersect(ray))
                return Intersection();
            //cout << rt << endl;
            //cout << rt->bbox.lo << rt->bbox.hi << endl;
            Intersection inter = rt->facet.intersect(ray, color), interl, interr;
            if(rt->l)
                interl = find(rt->l, ray);
            if(rt->r)
                interr = find(rt->r, ray);
            if(!inter.N.nonzero()) {
                if(interl.N.nonzero())
                    inter = interl;
                if(interr.N.nonzero())
                    inter = interr;
            }
            if(inter.N.nonzero()) {
                if(interl.N.nonzero()) {
                    if((inter.pos - ray.pos).L2sqr() > (interl.pos - ray.pos).L2sqr()) {
                        inter = interl;
                    }
                }
                if(interr.N.nonzero()) {
                    if((inter.pos - ray.pos).L2sqr() > (interr.pos - ray.pos).L2sqr()) {
                        inter = interr;
                    }
                }
            }
            return inter;
        }
        Intersection Intersect(const Ray &ray)const {
            return find(root, ray);
        }
    } aabbtree;

public:
    TriangleMesh(const std::string &_fn, Vec3 _color);
    virtual Intersection intersect(const Ray &_ray);
    ~TriangleMesh() {}
};

