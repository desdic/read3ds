#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "texture.h"

enum {SHADE_FLAT=1,SHADE_GOURAUD=2,SHADE_PHONG=3,SHADE_METAL=4};

class Material: private Texture {
public:
    Material();
    ~Material();
    void SetMaterialName(const char *name);
    void SetMapFilename(const char *name);
    void SetAmbient(const float red, const float green, const float blue);
    void SetDiffuse(const float red, const float green, const float blue);
    void SetSpecular(const float red, const float green, const float blue);
    void SetTransparentcy(const float alpha);
    void SetShading(const unsigned short shade);

    char *GetMapFilename() const;
    char *GetMaterialName() const;
    float *GetAmbient() const;
    float *GetDiffuse() const;
    float *GetSpecular() const;
    float GetTransparentcy() const;
    unsigned short GetShading() const;

    GLuint GetTextureID() const;

private:
    GLuint textureid;
    char *materialname;
    char *mapfile;
    float *ambient;
    float *diffuse;
    float *specular;
    float transparentcy;
    unsigned short shading;
};

#endif
