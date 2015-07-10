#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <vector>
#include "ivector.h"
#include "texture.h"
#include "iface.h"
#include "mapping.h"

typedef struct iobject {
    char *name;

    int numvertics;
    int numfaces;
    ivertex *vertics;
    ivector3 *normals;
    iface *faces;
    iuv *uvs;

    float rotx, roty, rotz;
    // TODO: rotate matrix
    float scalex, scaley, scalez;
    float pivotx,pivoty,pivotz;
    float posx, posy, poxz;

    int hidden;

} iobject;

typedef struct iscene {
    
    std::vector<iobject> objects;

    Texture tex;

} iscene;

#endif
