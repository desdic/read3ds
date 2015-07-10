#include <string.h>
#include "camera.h"

Camera::Camera() {
    pos = ivector3(0,0,0);
    target = ivector3(0,0,0);
    windowheight = 640;
    windowwidth = 480;
    fov = 45.0f;
    znear = 5.0f;
    zfar = 10000.0f;       
}

Camera::~Camera() {
    if (cameraname!=NULL)
        delete [] cameraname;
}

void Camera::SetName(const char *name) {
    cameraname = new char[strlen(name)+1];
    strcpy(cameraname, name);
}

void Camera::SetPos(const ivector3 p) {
    pos = p;
}

void Camera::SetTarget(const ivector3 t) {
    target = t;
}

void Camera::SetFOV(const double f) {
    fov = f;
}

void Camera::SetWindowHW(const double height, const double width) {
    windowheight = height;
    windowwidth = width;
}

void Camera::SetZNear(const double near) {
    znear = near;
}

void Camera::SetZFar(const double far) {
    zfar = far;
}

char *Camera::GetName() const {
    return cameraname;
}

ivector3 Camera::GetPos() const {
    return pos;
}

ivector3 Camera::GetTarget() const {
    return target;
}

double Camera::GetFOV() const {
    return fov;
}

double Camera::GetWindowHeight() const {
    return windowheight;
}

double Camera::GetWindowWidth() const {
    return windowwidth;
}

double Camera::GetZNear() const {
    return znear;
}

double Camera::GetZFar() const {
    return zfar;
}


