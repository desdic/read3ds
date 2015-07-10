#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>
#include "object.h"
#include "material.h"
#include "camera.h"
#include "shader.h"
#include "config.h"

class Scene {
public:
    Scene();
    ~Scene();

    void dump();

    void SetActiveCam(const char *camname);
    void InitScene(const Config &c);

    std::vector<Object*> Objects;
    std::vector<Material*> Materials;
    std::vector<Camera *> Cameras;
    std::vector<Shader *> Shaders;

    Camera *activecam;

};

#endif
