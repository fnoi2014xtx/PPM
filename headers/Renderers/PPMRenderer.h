#pragma once
#include "../Renderer.h"
#include "../Ray.h"
#include "../Object.h"
#include "../Scene.h"
#include <vector>
#include <algorithm>
struct HitPoint {
    Vec3 pos, N;
    Object *obj = nullptr;
    int row, col;
    Vec3 weight;
    Vec3 tau;
    double r;
    double totalN = 0, tmpN = 0;
    void update(double alpha) {
        if(tmpN <= 0)return;
        double k = sqrt((totalN + alpha * tmpN) / (totalN + tmpN));
        r *= k;
        tau *= k * k;
        totalN += tmpN * alpha;
        tmpN = 0;
    }
};
typedef HitPoint *HitPointPointer;
struct Photon {
    Vec3 pos, dir, from;
    Vec3 weight;
    Object *obj = nullptr;
};


struct KDTree {
    struct Node {
        Node *l = nullptr, *r = nullptr;
        double maxRadius = 0;
        HitPoint *hp = nullptr;
        Vec3 lo, hi;
        void update() {
            //assert(hp != nullptr);
            maxRadius = hp->r;
            lo = hi = hp->pos;
            if(l != nullptr) {
                lo = min(l->lo, lo);
                hi = max(l->hi, hi);
                maxRadius = std::max(maxRadius, l->maxRadius);
            }
            if(r != nullptr) {
                lo = min(r->lo, lo);
                hi = max(r->hi, hi);
                maxRadius = std::max(maxRadius, r->maxRadius);
            }
        }
    } *root = nullptr, *pond = nullptr;
    int pondcnt = 0;
    HitPoint *tmp = nullptr;
    void init(std::vector<HitPoint> &data) {
        int n = data.size();
        pond = new Node[n];
        tmp = data.data();
        root = build(0, n - 1, 0);
    }
    Node *newnode(HitPoint *hp) {
        Node *rt = &pond[pondcnt++];
        rt->hp = hp;
        rt->update();
        return rt;
    }
    static int OPT;
    static bool cmp(const HitPoint &a, const HitPoint &b) {
        return (a.pos.at(OPT) < b.pos.at(OPT));
    }
    Node *build(int l, int r, int dim) {
        int mid = (l + r) >> 1;
        OPT = dim;
        std::nth_element(tmp + l, tmp + mid, tmp + r + 1, cmp);
        Node *ret = newnode(&tmp[mid]);
        if(l < mid)ret->l = build(l, mid - 1, (dim + 1) % 3);
        if(mid < r)ret->r = build(mid + 1, r, (dim + 1) % 3);
        ret->update();
        return ret;
    }

    void insertPhoton(Node *rt, Photon &photon) {
        double delta = 0.0;
        for(int i = 0; i < 3; i++) {
            if(photon.pos[i] < rt->lo[i])
                delta += (rt->lo[i] - photon.pos[i]) * (rt->lo[i] - photon.pos[i]);
            if(photon.pos[i] > rt->hi[i])
                delta += (photon.pos[i] - rt->hi[i]) * (photon.pos[i] - rt->hi[i]);
        }
        delta = sqrt(delta);
        if(delta > rt->maxRadius + EPS)
            return;
        if((photon.pos - rt->hp->pos).L2() < rt->hp->r && photon.obj == rt->hp->obj) {
            rt->hp->tmpN++;
            rt->hp->tau += photon.weight;
        }
        if(rt->l != nullptr)
            insertPhoton(rt->l, photon);
        if(rt->r != nullptr)
            insertPhoton(rt->r, photon);
    }
    void InsPhoton(Photon &photon) {
        insertPhoton(root, photon);
    }
    void updateState(Node *rt) {
        if(rt->l)
            updateState(rt->l);
        if(rt->r)
            updateState(rt->r);
        rt->hp->update(ALPHA);
        rt->update();
    }
    double ALPHA;
    void UpdateState(double alpha) {
        ALPHA = alpha;
        updateState(root);
    }
};


class PPMRenderer : public Renderer {

    std::vector<HitPoint> hitpoints;
    std::vector<HitPoint> bghitpoints;

    KDTree kdtree;

    void RTPass(HitPoint hitpoint, const Ray &ray, int depth);
    void PTPass(Photon &photon, int depth);
    void EvalIrradiance(int iter);
    double ALPHA = PPM_ALPHA;
    int DEPTH_MAX = PPM_DEPTH;
    int MAX_ITER = PPM_ITER;
    int MAX_PHOTON_NUM = PPM_PHOTON;
    double INIT_RADIUS = PPM_INIT_RADIUS;
    double illumination = 6000;
public:
    PPMRenderer() : Renderer() {};
    virtual void render(Scene *scene);
    void setDepthMax(int _DEPTH_MAX) {
        DEPTH_MAX = _DEPTH_MAX;
    }
    void setMaxIter(int _MAX_PPM_ITER) {
        MAX_ITER = _MAX_PPM_ITER;
    }
    void setMaxPhotonNum(int _MAX_PHOTON_NUM) {
        MAX_PHOTON_NUM = _MAX_PHOTON_NUM;
    }
    void setInitRadius(double _INIT_RADIUS) {
        INIT_RADIUS = _INIT_RADIUS;
    }
    void setAlpha(double _alp) {
        ALPHA = _alp;
    }
    void setIllumination(double il) {
        illumination = il;
    }
    ~PPMRenderer() {};
};

