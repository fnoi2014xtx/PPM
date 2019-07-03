#include <algorithm>
#include <iostream>
#include "../headers/Scene.h"
#include "../headers/utils.h"
void Scene::addLight(Light *light) {
    lights.push_back(light);
}

void Scene::addObject(Object *object) {
    objects.push_back(object);
}

void Scene::setCamera(Camera *_camera) {
    if(camera)
        delete camera;
    camera = _camera;
}

void Scene::setRenderer(Renderer *_renderer) {
    if(renderer)
        delete renderer;
    renderer = _renderer;
}

void Scene::exportImg(const std::string &name) {
#define crop01(x) std::min(std::max(x, 0.0), 1.0)
    if (!renderer->img)
        throw "Error! You need to render the Img first!";
    for(size_t i = 0; i < HEIGHT; i++)
        for(size_t j = 0; j < WIDTH; j++) {
            renderer->img[i][j].x = crop01(renderer->img[i][j].x);
            renderer->img[i][j].y = crop01(renderer->img[i][j].y);
            renderer->img[i][j].z = crop01(renderer->img[i][j].z);
        }
#undef crop01
    PNGImage::write(name, HEIGHT, WIDTH, renderer->img);
}

void Scene::render() {
    if(!camera || !renderer)
        throw "Error!You need to set a camera and a renderer first!";
    renderer->render(this);
}
