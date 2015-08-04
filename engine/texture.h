#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <vector>

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

enum { 
    TEXTURE_SCALE=1, TEXTURE_MIPMAP=2, TEXTURE_ALPHA=4, TEXTURE_MODULATE=8, TEXTURE_REPLACE=16, TEXTURE_REPEAT=32
};

class Texture {
public:
    Texture();
    virtual ~Texture();
    int CreateEmptyTexture(unsigned int sizex, unsigned int sizey);
    int LoadTexture(const char *filename);
    void SetNumMipmaps(const int n);
    void SetFlags(const int f);
private:
    int flags;
    int nummipmaps;
    std::vector<GLuint> texs;
};

#endif
