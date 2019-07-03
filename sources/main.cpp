#include <iostream>
#include <string>
#include "../headers/Scene.h"
#include "../headers/Camera.h"
#include "../headers/Objects/utils.h"
#include "../headers/Texture.h"
#include "../headers/Materials/utils.h"
#include "../headers/Renderers/utils.h"
#include "../headers/Lights/utils.h"
using namespace std;

typedef void (*CommandFuction) ();
struct Command {
    const char *text;
    CommandFuction func;
    const char *description;
};

void Mesh();
void Bezier();
void ACube();
void ASphere();
void Result();
const Command Commands[] = {
    {"mesh", Mesh, "Generates an image of a TriangleMesh.\n"},
    {"bezier", Bezier, "Bezier2D .\n"},
    {"cube", ACube, "A lonely cube.\n"},
    {"sphere", ASphere, "A lonely sphere.\n"},
    {"result", Result, "Result.png"}
};
const size_t CommandsNum = sizeof(Commands) / sizeof(Commands[0]);
void PrintUsageText() {
    cout << "Computer Graphics Assignment\n"
         << "Support multiple rendering methods\n"
         << "developed by xtx\n"
         << "The following command line options are supported:\n";
    for (size_t i = 0; i < CommandsNum; i++) {
        cout << Commands[i].text << ":" << Commands[i].description;
    }
}
int main(int argc, const char *argv[]) {

    if (argc == 1) {
        PrintUsageText();
    } else {
        const string arg_text = argv[1];
        bool flag = false;
        for (size_t i = 0; i < CommandsNum; i++)
            if (arg_text == Commands[i].text) {
                flag = true;
                Commands[i].func();
                break;
            }
        if (!flag) {
            cerr << "Error! Unknown command line option!" << endl;
            return 1;
        }
    }
    return 0;
}

void Result() {
    Scene scene(Vec3(0, 0, 0));
    Object *Ground = new Plane(Vec3(1, 1, 1), Vec3(0, 0, 1), Vec3(1, 0, 0), 80, 50);
    Ground->setMaterial(new Plastic());
    Ground->setPos(Vec3(-25, 0, 0));
    Ground->setTexture(new Texture("./images/G.png"));
    scene.addObject(Ground);
    Object *Ceil = new Plane(Vec3(1, 1, 1), Vec3(1, 0, 0), Vec3(0, 0, 1), 50, 80);
    Ceil->setMaterial(new Porcelain());
    Ceil->setPos(Vec3(-25, 40, 0));
    scene.addObject(Ceil);
    Object *Left = new Plane(Vec3(0.5, 1, 0), Vec3(0, 1, 0), Vec3(0, 0, 1), 40, 80);
    Left->setMaterial(new Plastic());
    Left->setPos(Vec3(-25, 0, 0));
    Left->setTexture(new Texture("./images/L.png"));
    scene.addObject(Left);
    Object *Right = new Plane(Vec3(1, 0.5, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), 80, 40);
    Right->setMaterial(new Plastic());
    Right->setPos(Vec3(25, 0, 0));
    Right->setTexture(new Texture("./images/R.png"));
    scene.addObject(Right);
    Object *Middle = new Plane(Vec3(1, 1, 1), Vec3(1, 0, 0), Vec3(0, 1, 0), 50, 40);
    Middle->setMaterial(new Plastic());
    Middle->setPos(Vec3(-25, 0, 0));
    Middle->setTexture(new Texture("./images/M.png"));
    scene.addObject(Middle);


    Vec3 ps[16] = {
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(1.3, 0.7, -1.3),
        Vec3(1.4, 0, -0.5),
        Vec3(1.35, 0, 0.7),
        Vec3(1.37, 0.3, 2),
        Vec3(3, 1, -1.8),
        Vec3(2.8, 0, -0.6),
        Vec3(2.5, 0.3, 0.8),
        Vec3(2.7, 0.6, 3),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
    };
    Object *b = new Bezier3(Vec3(1, 1, 1), ps);
    b->setMaterial(new Plastic());
    b->setScale(2);
    b->setPos(Vec3(-15, 0, 25));
    b->setTexture(new Texture("./images/leaf.png"));
    b->setScale(1.3);
    scene.addObject(b);




    Object *ball = new Sphere(Vec3(0.7, 0.7, 1), 6);
    ball->setMaterial(new Glass());
    ball->setPos(Vec3(-10, 25, 50));
    scene.addObject(ball);

    Object *mesh = new TriangleMesh("models/bunny.obj", Vec3(1, 1, 1));
    mesh->setMaterial(new Plastic());
    mesh->setScale(100);
    mesh->setPos(Vec3(13, -3.3333, 15));
    scene.addObject(mesh);









    Object *ball2 = new Sphere(Vec3(1, 1, 1), 3.5);
    ball2->setMaterial(new Glass());
    ball2->setPos(Vec3(-3.5, 3.5, 50));
    scene.addObject(ball2);
    Light *light = new PointLight(Vec3(0, 39, 25), Vec3(0.3, 0.3, 0.3));
    scene.addLight(light);
    //light = new PointLight(Vec3(10, 37, 25), Vec3(0.3, 0.3, 0.3));
    //scene.addLight(light);

    //light = new PointLight(Vec3(-5, 37, 25), Vec3(1.0, 1.0, 1.0));
    //scene.addLight(light);
    //light = new PointLight(Vec3(30, 0, 20), Vec3(1.0, 1.0, 1.0));
    Renderer *renderer = new PPMRenderer();
    scene.setRenderer(renderer);
    Camera *camera = new Camera(Vec3(0, 25, 120));
    //Camera *camera = new Camera(600, 800, Vec3(0, 150, 50));
    camera->lookAt(Vec3(0, 0, 0));
    //camera->lookAt(Vec3(-15, 0, 20), 0);
    scene.setCamera(camera);
    scene.render();
    scene.exportImg("./Results/Result.png");

}




void Mesh() {
    Scene scene(Vec3(0, 0, 0));
    Object *mesh = new TriangleMesh("models/kitten.obj", Vec3(1, 1, 1));
    mesh->setMaterial(new Plastic());
    mesh->setScale(0.2);
    mesh->setPos(Vec3(0, 0, -60));
    scene.addObject(mesh);
   
    Light *light = new PointLight(Vec3(-35, 40, 0), Vec3(1.0, 1.0, 1.0));
    scene.addLight(light);
    light = new PointLight(Vec3(30, 0, 20), Vec3(1.0, 1.0, 1.0));
    Renderer *renderer = new PPMRenderer();
    scene.setRenderer(renderer);
    Camera *camera = new Camera(Vec3(-30, 40, 0));
    camera->lookAt(Vec3(0, 3, -60));
    scene.setCamera(camera);
    scene.render();
    scene.exportImg("./Results/Mesh.png");
}
void Bezier() {
    Scene scene(Vec3(0, 0, 0));
    Vec3 ps[16] = {
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(0, 0.5, 0),
        Vec3(1.3, 0.7, -1.3),
        Vec3(1.4, 0, -0.5),
        Vec3(1.35, 0, 0.7),
        Vec3(1.37, 0.3, 2),
        Vec3(3, 1, -1.8),
        Vec3(2.8, 0, -0.6),
        Vec3(2.5, 0.3, 0.8),
        Vec3(2.7, 0.6, 3),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
        Vec3(4, 1.2, -1),
    };
    Object *b = new Bezier3(Vec3(1, 1, 1),ps);
    b->setMaterial(new Plastic());
    b->setPos(Vec3(-15, 0, 25));
    b->setTexture(new Texture("./images/leaf.png"));
    b->setScale(5);
    scene.addObject(b);
    Light *light = new PointLight(Vec3(0, 39, 25), Vec3(1, 1, 1));
    scene.addLight(light);
    Renderer *renderer = new PPMRenderer();
    scene.setRenderer(renderer);
    Camera *camera = new Camera(Vec3(-10, 20, 120));
    camera->lookAt(Vec3(-15, 0, 25), 0);
    scene.setCamera(camera);
    scene.render();
    scene.exportImg("./Results/Bezier.png");
}
void ACube() {
    Scene scene(Vec3(0, 0, 0));
   
    Object *cube = new Cube(Vec3(1, 1, 1), 8);
    cube->setMaterial(new Plastic());
    cube->setPos(Vec3(0, 0, -60));
    scene.addObject(cube);
    Object *sphere = new Sphere(Vec3(0.3, 0.5, 0.8), 10);
    sphere->setMaterial(new Mirror());
    sphere->setPos(Vec3(5, 0, -80));
    scene.addObject(sphere);
   

    Light *light = new PointLight(Vec3(5, 40, 10), Vec3(1.0, 1.0, 1.0));
    scene.addLight(light);
    light = new PointLight(Vec3(-10, 40, 10), Vec3(1.0, 1.0, 1.0));
    Renderer *renderer = new PPMRenderer();
    scene.setRenderer(renderer);
    Camera *camera = new Camera(Vec3(30, 40, 0));
    camera->lookAt(Vec3(0, 0, -60));
    scene.setCamera(camera);
    scene.render();
    scene.exportImg("./Results/ACube.png");
}
void ASphere() {
    Scene scene(Vec3(0, 0, 0));
    Object *sphere = new Sphere(Vec3(1, 1, 1), 3.5);
    sphere->setPos(Vec3(3, 5, -40));
    sphere->setMaterial(new Glass());
    //sphere->setScale(2);
    scene.addObject(sphere);
    Texture *text = new Texture("./images/timg.png");
    Object *plane;
    plane = new Plane(Vec3(1, 0, 0), Vec3(1, 0, 0), Vec3(0, 0, -1), 20, 100);
    plane->setPos(Vec3(-10, -10, 35));
    plane->setMaterial(new Plastic());
    scene.addObject(plane);
    plane = new Plane(Vec3(0, 1, 0), Vec3(1, 0, 0), Vec3(0, 1, 0), 20, 20);
    plane->setPos(Vec3(-10, -10, -65));
    plane->setTexture(text);
    plane->setMaterial(new Plastic());
    scene.addObject(plane);


    plane = new Plane(Vec3(0, 0, 1), Vec3(0, 0, -1), Vec3(0, 1, 0), 100, 20);
    plane->setPos(Vec3(-10, -10, 35));
    plane->setMaterial(new Plastic());
    scene.addObject(plane);

    plane = new Plane(Vec3(0, 1, 1), Vec3(0, 1, 0), Vec3(0, 0, -1), 20, 100);
    plane->setPos(Vec3(10, -10, 35));
    plane->setMaterial(new Plastic());
    scene.addObject(plane);

    Light *light = new PointLight(Vec3(5, 40, 60), Vec3(1.0, 1.0, 1.0));
    scene.addLight(light);
    Renderer *renderer = new PPMRenderer();
    scene.setRenderer(renderer);
    Camera *camera = new Camera(Vec3(5, 0, 0));
    camera->lookAt(Vec3(0, 0, -60));
    scene.setCamera(camera);
    scene.render();
    scene.exportImg("./Results/ASphere.png");
}