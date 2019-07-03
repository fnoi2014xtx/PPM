#pragma once
#include "Light.h"
#include "Camera.h"
#include "Object.h"
#include "Renderer.h"
#include "Vec3.h"
#include <vector>
#include <string>
#include <iostream>
class Scene {
public:
    Camera *camera;
    std::vector<Light *> lights;
    std::vector<Object *> objects;
    Renderer *renderer;
    Vec3 background;
    Scene(Vec3 _background) : background(_background), camera(nullptr), renderer(nullptr) {}
    void addObject(Object *obj);
    void addLight(Light *light);
    void setCamera(Camera *_camera);
    void setRenderer(Renderer *_renderer);
    void render();
    void exportImg(const std::string &name);
    ~Scene() {
        if(camera)
            delete camera;

        if(renderer)
            delete renderer;
        for (auto light : lights) {
            delete light;
        }
        for (auto object : objects) {
            delete object;
        }
    }
};

