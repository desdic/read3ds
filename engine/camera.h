
#include "ivector.h"

#ifndef __CAMERA_H__
#define __CMAERA_H__

class Camera {
public:
    Camera();
    ~Camera();

    void SetName(const char *name);
    void SetPos(const ivector3 p);
    void SetTarget(const ivector3 t);
    void SetFOV(const double f);
    void SetWindowHW(const double height, const double width);
    void SetZNear(const double near);
    void SetZFar(const double far);

    char *GetName() const;
    ivector3 GetPos() const;
    ivector3 GetTarget() const;
    double GetFOV() const;
    double GetWindowHeight() const;
    double GetWindowWidth() const;
    double GetZNear() const;
    double GetZFar() const;
        
private:
    char *cameraname;
    int id;
    ivector3 pos, target;
    double fov;
    double windowheight, windowwidth;
    double znear, zfar;
};

#endif
