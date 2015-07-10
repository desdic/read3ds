#ifndef __drawing_cc__
#define __drawing_cc__

#include <GL/gl.h>												
#include <GL/glu.h>												

void ViewOrtho(int height, int width);
void ViewPerspective();
void DrawRadialBlur(int times, float inc, int height, int width, GLfloat r, GLfloat g, GLfloat b, GLuint BLURTEXTURE);
void putTransImage(int image, float x, float y, float z, float blend, char test);

#endif
