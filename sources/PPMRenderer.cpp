#include "../headers/Renderers/PPMRenderer.h"
#include <cstdlib>
#include <cstring>
#include <omp.h>
#include <ctime>
//int PPMRenderer::KDTree OPT=0;
int KDTree::OPT = 0;
void PPMRenderer::render(Scene *scene) {
    sc = scene;
    img = new Vec3* [HEIGHT];
    for(int i = 0; i < HEIGHT; i++) {
        img[i] = new Vec3[WIDTH];
    }
    std::cout << "PPM Renderer starts" << std::endl;
    int starttime = clock();
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++) {
            for(int k = 0; k < sc->camera->sampleNum(); k++) {
                Ray ray = sc->camera->distributedray(i, j);
                //img[i][j] += RayTrace(ray, 0);
                //Ray ray = scene->camera->ray(i, j);
                HitPoint hitpoint;
                hitpoint.row = i;
                hitpoint.col = j;
                hitpoint.weight = Vec3(1, 1, 1) / sc->camera->sampleNum();
                RTPass(hitpoint, ray, 0);
            }
            //img[i][j] /= sc->camera->SampleNum;
        }
    cout << "Ray Tracing Pass Finish!" << endl;
    kdtree.init(hitpoints);
    for (int i = 0; i < MAX_ITER; i++) {
        double total = 0.0;
        for (Light *light : sc->lights) {
            Vec3 tmp = light->Color();
            total += (tmp.x + tmp.y + tmp.z) / 3;
        }
        //srand(time(0));
        double photonPower = total / MAX_PHOTON_NUM;
        for (Light *light : sc->lights) {
            Vec3 tmp = light->Color();
            int photon_num = int((tmp.x + tmp.y + tmp.z) / (photonPower * 3));
            Vec3 photonColor = light->Color() / photon_num;
            #pragma omp parallel num_threads(32)
            {
                srand(int(time(NULL))^omp_get_thread_num());
                #pragma omp for schedule(dynamic,1)
                for (int j = 0; j < photon_num; j++) {
                    //if (j % 100000 == 0) cout << "j = " << j << endl;
                    Photon photon;
                    photon.from = light->randomPoint();
                    photon.dir = Vec3::Random();
                    photon.weight = photonColor;
                    PTPass(photon, 0);
                }
            }
        }
        //cout << "Elapsed time: " << (clock() - startTime) / CLOCKS_PER_SEC << "s." << endl;
        kdtree.UpdateState(ALPHA);
        EvalIrradiance(i + 1);
        if((i + 1) % 50 == 0) {
            char name[50];
            sprintf(name, "./Results/%d.png", i + 1);
            cout << name << endl;
            sc->exportImg(name);
        }
        cout << "i=" << i << endl;
    }
    int endtime = clock();
    std::cout << "PPM Renderer ends" << std::endl;
    std::cout << "Renderer running time: " << (endtime - starttime) / CLOCKS_PER_SEC << "s" << std::endl;
}
void PPMRenderer::RTPass(HitPoint hitpoint, const Ray &ray, int depth) {
    if(depth > DEPTH_MAX) {
        bghitpoints.push_back(hitpoint);
        return;
    }
    Intersection intersection;
    double minl = INF;
    for (Object *obj : sc->objects) {
        Intersection inter = obj->intersect(ray);
        if (inter.obj != nullptr) {
            Vec3 d = inter.pos - ray.pos;
            double l = d.L2();
            if (l < minl) {
                minl = l;
                intersection = inter;
            }
        }
    }
    if(intersection.type == MISS) {
        bghitpoints.push_back(hitpoint);
        return;
    }
    Material *mt = intersection.obj->mat();
    if(mt->diff() > EPS) {
        HitPoint h = hitpoint;
        h.obj = intersection.obj;
        h.pos = intersection.pos;
        h.N = intersection.N;
        h.weight &= (intersection.objColor * mt->diff());
        h.r = INIT_RADIUS;
        hitpoints.push_back(h);
    }
    if(mt->refl() > EPS) {
        Ray refl_ray;
        refl_ray.dir = ray.dir.reflected(intersection.N);
        refl_ray.pos = intersection.pos + EPS * refl_ray.dir;
        HitPoint h = hitpoint;
        h.weight &= (intersection.objColor * mt->refl());
        RTPass(h, refl_ray, depth + 1);
    }
    if(mt->refr() > EPS) {
        Ray refr_ray;
        double n = (intersection.type == IN) ? mt->refr_index() : 1 / mt->refr_index();
        refr_ray.dir = ray.dir.refracted(intersection.type == IN ? -intersection.N : intersection.N, n);
        refr_ray.pos = intersection.pos;
        //Ray refr_ray;
        //refr_ray.dir = ray.dir.refracted(intersection.N, intersection.type == IN ? 1.0 / mt->refr_index() : mt->refr_index());
        //refr_ray.pos = intersection.pos + EPS * refr_ray.dir;
        HitPoint h = hitpoint;
        h.weight &= (intersection.objColor * mt->refr());
        RTPass(h, refr_ray, depth + 1);
    }
}
void PPMRenderer::PTPass(Photon &photon, int depth) {
    if(depth > DEPTH_MAX)return;
    Intersection intersection;
    double minl = INF;
    for (Object *obj : sc->objects) {
        Ray ray;
        ray.pos = photon.from;
        ray.dir = photon.dir;
        Intersection inter = obj->intersect(ray);
        if (inter.obj != nullptr) {
            Vec3 d = inter.pos - ray.pos;
            double l = d.L2();
            if (l < minl) {
                minl = l;
                intersection = inter;
            }
        }
    }
    if(intersection.type == MISS)return;
    photon.pos = intersection.pos;
    photon.obj = intersection.obj;
    if(intersection.obj->mat()->diff() > EPS)
        kdtree.InsPhoton(photon);
    Photon newphoton = photon;
    newphoton.weight &= intersection.objColor;
    newphoton.from = intersection.pos;
    Material *mt = intersection.obj->mat();
    double rd = 1.0 * rand() / double(RAND_MAX);
    if(rd < mt->diff()) {
        newphoton.dir = Vec3::RandomCos(intersection.N);

    } else if(rd < mt->diff() + mt->refl()) {
        newphoton.dir = photon.dir.reflected(intersection.N);
    } else {
        double n = (intersection.type == IN) ? mt->refr_index() : 1 / mt->refr_index();
        newphoton.dir = photon.dir.refracted(intersection.type == IN ? -intersection.N : intersection.N, n);
    }
    PTPass(newphoton, depth + 1);
}


void PPMRenderer::EvalIrradiance(int iter) {
    for(int i = 0; i < HEIGHT; i++)
        for(int j = 0; j < WIDTH; j++)
            img[i][j].x = img[i][j].y = img[i][j].z = 0;
    for (int i = 0; i < kdtree.pondcnt; i++) {
        HitPoint *hp = &kdtree.tmp[i];
        Vec3 irradiance = illumination * hp->tau / (hp->r * hp->r * iter);
        img[hp->row][hp->col] += (irradiance & hp->weight);
    }

    Vec3 color = sc->background;
    for (HitPoint &hp : bghitpoints) {
        img[hp.row][hp.col] += color * hp.weight;
    }
}
