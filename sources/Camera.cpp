#include "../headers/Camera.h"

Ray Camera::ray(double x, double y) {
    x += shiftH;
    y += shiftW;
    return { pos, (F + H * (2 * x / HEIGHT - 1) + W * (2 * y / WIDTH - 1)).normalized()};
}
void Camera::lookAt(const Vec3 &p, int _shiftH, int _shiftW) {
    Vec3 up(0, 1, 0);
    F = (p - pos).normalized() * Lens;
    W = (F * up).normalized() * WIDTH;
    H = -(W * F).normalized() * HEIGHT;
    FocalDis = pos.z - p.z;
    shiftH = _shiftH;
    shiftW = _shiftW;
}
Ray Camera::distributedray(double x, double y) {
    x += shiftH;
    y += shiftW;
    Vec3 emitDir = F + H * (2 * x / HEIGHT - 1) + W * (2 * y / WIDTH - 1);
    Vec3 target = pos + emitDir * FocalDis / -emitDir.z;
    double detH, detW;
    do {
        detH = rand01() * 2.0 - 1.0;
        detW = rand01() * 2.0 - 1.0;
    } while (detH * detH + detW * detW < EPS || detH * detH + detW * detW >= 1.0);
    Vec3 unitH = H.normalized(), unitW = W.normalized();
    Ray ray;
    ray.pos = pos + (unitH * detH + unitW * detW) * Aperture;
    ray.dir = (target - ray.pos).normalized();
    return ray;
}