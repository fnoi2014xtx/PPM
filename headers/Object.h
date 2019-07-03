#pragma once
#include "Material.h"
#include "Texture.h"
#include "Vec3.h"
#include "Ray.h"
#include "Intersection.h"
class Object {
protected:
    Texture *texture;
    Material *material;
    Vec3 color;
    double scale;
    Vec3 pos;
    Vec3 Xdir, Ydir, Zdir;
    Ray localRay(const Ray &ray)const {
        double dis = (ray.pos - pos).L2() / scale;
        double xx = (ray.pos - pos)^Xdir;
        double yy = (ray.pos - pos)^Ydir;
        double zz = (ray.pos - pos)^Zdir;
        Ray ret;
        ret.pos = Vec3(xx, yy, zz).normalized() * dis;
        ret.dir = Vec3(ray.dir ^ Xdir, ray.dir ^ Ydir, ray.dir ^ Zdir).normalized();
        return ret;
    }
    Intersection globalIntersection(const Intersection &inter) {
        Intersection ret;
        if(inter.type == MISS)
            return ret;
        ret.obj = this;
        ret.pos = pos + inter.pos.x * Xdir * scale + inter.pos.y * Ydir * scale + inter.pos.z * Zdir * scale;
        ret.N = (inter.N.x * Xdir + inter.N.y * Ydir + inter.N.z * Zdir).normalized();
        ret.objColor = inter.objColor;
        ret.type = inter.type;
        return ret;
    }
public:
    Object(Vec3 _color) : material(nullptr), texture(nullptr), pos(Vec3(0, 0, 0)), color(_color), scale(1.0), Xdir(Vec3(1, 0, 0)), Ydir(Vec3(0, 1, 0)), Zdir(Vec3(0, 0, 1)) {}
    ~Object() {
        if(material != nullptr)
            delete material;
        if(texture != nullptr)
            delete texture;
    }
    void setTexture(Texture *_t) {
        if (texture)
            delete texture;
        texture = _t;
    }
    void setMaterial(Material *_m) {
        if (material)
            delete material;
        material = _m;
    }
    void setPos(const Vec3 &_p) {
        pos = _p;
    }
    void setScale(const double &sc) {
        scale = sc;
    }
    void setAxis(const Vec3 &_x, const Vec3 &_y, const Vec3 &_z) {
        Xdir = _x.normalized();
        Ydir = _y.normalized();
        Zdir = _z.normalized();
    }
    Material *mat()const {
        return material;
    }
    virtual Intersection intersect(const Ray &_ray) = 0;
};

