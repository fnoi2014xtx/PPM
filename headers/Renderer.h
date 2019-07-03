#pragma once
#include "Vec3.h"
class Scene;
class Renderer {
protected:
    Scene *sc = nullptr;
public:
    Vec3 **img = nullptr;
    Renderer() {};
    virtual void render(Scene *scene) = 0;
    ~Renderer() {
        if (!img)
            return;
        for (int i = 0; i < HEIGHT; i++)
            delete[] img[i];
        delete[]img;
    };
};

