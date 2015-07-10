#include <string.h>
#include <stdio.h>
#include "material.h"

Material::Material() {
    transparentcy = 1.0f;
    materialname = mapfile = NULL;
    ambient = diffuse = specular = NULL;
    shading = SHADE_PHONG;
    textureid = -1;
}

Material::~Material() {

    if (materialname!=NULL)
        delete [] materialname;

    if (mapfile!=NULL)
        delete [] mapfile;

    if (ambient!=NULL)
        delete [] ambient;

    if (diffuse!=NULL)
        delete [] diffuse;

    if (specular!=NULL)
        delete [] specular;
}

void Material::SetMaterialName(const char *name) {
    materialname = new char [strlen(name)+1];
    strncpy(materialname, name, strlen(name)+1);
    materialname[strlen(name)+1]=0;
}

void Material::SetMapFilename(const char *name) {
    mapfile = new char [strlen(name)+1];
    strncpy(mapfile, name, strlen(name)+1);
    mapfile[strlen(name)+1]=0;

    // no texture
    if (mapfile==NULL) {
        textureid=-1;
        return;
    }

    textureid = LoadTexture(name);

    if (textureid<1) {
        printf("Warning: Texture does not exists! %s\n", name);
    }
}

void Material::SetAmbient(const float red, const float green, const float blue) {
    ambient = new float[4];
    ambient[0] = red;
    ambient[1] = green;
    ambient[2] = blue;
    ambient[4] = 1.0f;
}

void Material::SetDiffuse(const float red, const float green, const float blue) {
    diffuse = new float[4];
    diffuse[0] = red;
    diffuse[1] = green;
    diffuse[2] = blue;
    diffuse[4] = 1.0f;
}

void Material::SetSpecular(const float red, const float green, const float blue) {
    specular = new float[4];
    specular[0] = red;
    specular[1] = green;
    specular[2] = blue;
    specular[4] = 1.0f;
}

void Material::SetTransparentcy(const float alpha) {
    transparentcy = alpha;
}

void Material::SetShading(const unsigned short shade) {
    shading = shade;
}

char *Material::GetMapFilename() const {
    return mapfile;
}

char *Material::GetMaterialName() const {
    return materialname;
}

float *Material::GetAmbient() const {
    return ambient;
}

float *Material::GetDiffuse() const {
    return diffuse;
}

float *Material::GetSpecular() const {
    return specular;
}

float Material::GetTransparentcy() const {
    return transparentcy;
}

unsigned short Material::GetShading() const {
    return shading;
}

GLuint Material::GetTextureID() const {
    return textureid;
}
