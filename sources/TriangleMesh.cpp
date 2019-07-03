#include "../headers/Objects/TriangleMesh.h"
#include <cstdio>
#include <cstring>
TriangleMesh::TriangleMesh(const std::string &_fn, Vec3 _color): Object(_color) {
    freopen(_fn.c_str(), "r", stdin);
    char opt[10];
    char buffer[1000];
    while(scanf("%s", opt) != EOF) {
        if(strlen(opt) != 1) {
            std::cin.getline(buffer, 1000);
            continue;
        }
        double x, y, z;
        int a, b, c;
        switch(opt[0]) {
        case 'v':
            scanf("%lf%lf%lf", &x, &y, &z);
            vertexs[n].x = x;
            vertexs[n].y = y;
            vertexs[n].z = z;
            n++;
            break;
        case 'f':
            scanf("%d%d%d", &a, &b, &c);
            facets[m].A = vertexs[a - 1];
            facets[m].B = vertexs[b - 1];
            facets[m].C = vertexs[c - 1];
            m++;
            break;
        default:
            std::cin.getline(buffer, 1000);
            break;
        }
    }
    fclose(stdin);
    cout << n << " " << m << endl;
    //for(int i=0;i<m;i++)cout<<facets[i].A<<facets[i].B<<facets[i].C<<endl;
    aabbtree.root = aabbtree.build(0, m - 1, facets);
    aabbtree.color = color;
    std::cout << "Init Triangle Mesh Finish! " << "BBox = " << aabbtree.root->bbox.lo << aabbtree.root->bbox.hi << std::endl;
}


Intersection TriangleMesh::intersect(const Ray &_ray) {
    Ray ray = localRay(_ray);
    Intersection intersection = aabbtree.Intersect(ray);
    return globalIntersection(intersection);
}
