#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "texture.h"

Texture::Texture() {
    flags = 0;
    flags = TEXTURE_ALPHA|TEXTURE_MODULATE|TEXTURE_REPEAT;
    nummipmaps = 3;
}

Texture::~Texture() {
    // Be nice an clean up

    for (std::vector<GLuint>::iterator p=texs.begin();p<texs.end();p++) {
        if (*p>0) {
            printf("Deleting texture %d\n", *p);
            glDeleteTextures(1,&*p);
        }
    }
}

void Texture::SetNumMipmaps(const int n) {
    nummipmaps = n;
}

void Texture::SetFlags(const int f) {
    flags = f;
}

int Texture::CreateEmptyTexture(unsigned int sizex, unsigned int sizey) {
    GLuint textureid;

    GLuint *data;

    if (flags & TEXTURE_ALPHA) {
        data = new GLuint[sizex*sizey*4*sizeof(GLuint)];
        memset(data, 0, sizex*sizey*4*sizeof(GLuint));
    } else {
        data = new GLuint[sizex*sizey*3*sizeof(GLuint)];
        memset(data, 0, sizex*sizey*3*sizeof(GLuint));
    }

    glGenTextures(1, &textureid);
    glBindTexture(GL_TEXTURE_2D, textureid);

    if (flags & TEXTURE_ALPHA) {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, sizex, sizey, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, 3, sizex, sizey, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    delete [] data;    

    texs.push_back( textureid );

    return textureid;    
   
}

int Texture::LoadTexture(const char *filename) {

    unsigned char *buffer;
    GLuint textureid;
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;

    if (filename==NULL)
        return -1;

    //printf("Loading %s\n", filename);

    loadedImage = IMG_Load( filename );    

    if (loadedImage == NULL) {
        printf("Warning unable to load %s\n", filename);
        return -1;
    }

    optimizedImage = SDL_DisplayFormat( loadedImage );

    if ( optimizedImage == NULL ) {
        printf("SDL_DisplayFormat failed\n");
        return -1;
    }

    if (flags & TEXTURE_ALPHA) {
        SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGBA( optimizedImage->format, 0xFF, 0xFF, 0xFF, 0xFF ) );
    } else {
        SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0xFF, 0xFF, 0xFF ) );
    }

    SDL_LockSurface(optimizedImage);

    int *surface = (int *)optimizedImage->pixels;

    if (flags & TEXTURE_ALPHA){
        buffer = new unsigned char [ optimizedImage->w * optimizedImage->h * 4];
        memcpy(buffer, surface, optimizedImage->w * optimizedImage->h * 4);
    } else {
        buffer = new unsigned char [ optimizedImage->w * optimizedImage->h * 4];
        memcpy(buffer, surface, optimizedImage->w * optimizedImage->h * 4);
    }
    
    glGenTextures(1, &textureid);
    
	glBindTexture (GL_TEXTURE_2D, textureid);

    if (flags & TEXTURE_REPEAT) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);    
    }

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

    if (flags & TEXTURE_MIPMAP) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);     
    }

    if (flags & TEXTURE_MODULATE) {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    }
    
    if (flags & TEXTURE_REPLACE) {
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
    }

    if (flags & TEXTURE_ALPHA)  {
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, optimizedImage->w, optimizedImage->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        gluBuild2DMipmaps(GL_TEXTURE_2D, nummipmaps, optimizedImage->w, optimizedImage->h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    } else {
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, optimizedImage->w, optimizedImage->h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
        gluBuild2DMipmaps(GL_TEXTURE_2D, nummipmaps, optimizedImage->w, optimizedImage->h, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    }

    SDL_UnlockSurface( optimizedImage );
    SDL_FreeSurface( loadedImage );        
    SDL_FreeSurface( optimizedImage );
    delete [] buffer;

    texs.push_back(textureid);
    
    return textureid;
}

