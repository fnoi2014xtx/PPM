#pragma once
class Material {
    double diffusion, reflection, refraction;
    double refraction_index;
    double reflection_index;
public:
    Material(double _d, double _rl, double _rr, double _rli, double _rri) :
        diffusion(_d), reflection(_rl), refraction(_rr),
        reflection_index(_rli), refraction_index(_rri) {}
    double diff() {
        return diffusion;
    }
    double refl() {
        return reflection;
    }
    double refr() {
        return refraction;
    }
    double refl_index() {
        return reflection_index;
    }
    double refr_index() {
        return refraction_index;
    }
    ~Material() {};
};

