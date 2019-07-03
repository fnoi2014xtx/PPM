#pragma once
#include "Vec3.h"
#include "Ray.h"
class Camera {
    Vec3 pos, F, H, W;
    int shiftH, shiftW;
    double Aperture;
    double FocalDis;
    double Lens;
    int SampleNum;
public:
    Camera(Vec3 _pos) : pos(_pos), shiftH(0), shiftW(0), Aperture(APERTURE), FocalDis(FOCALDIS), Lens(LENS), SampleNum(SAMPLENUM) {}
    void setPos(Vec3 _pos) {
        pos = _pos;
    }
    void setAperture(double _a) {
        Aperture = _a;
    }
    void setFocalDis(double _f) {
        FocalDis = _f;
    }
    void setLens(double _l) {
        Lens = _l;
    }
    void setSampleNum(int _s) {
        SampleNum = _s;
    }
    int sampleNum(){
        return SampleNum;
    }
    Ray ray(double x, double y);
    Ray distributedray(double x, double y);
    void lookAt(const Vec3 &p, int _shiftH = 0, int _shiftW = 0);
    ~Camera() {}
};

