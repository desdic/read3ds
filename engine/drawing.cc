#include "drawing.h"

void ViewOrtho(int height, int width)												// Set Up An Ortho View
{
	glMatrixMode(GL_PROJECTION);								// Select Projection
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
	glOrtho( 0, height , width , 0, -1, 1 );							// Select Ortho Mode (640x480)
	glMatrixMode(GL_MODELVIEW);									// Select Modelview Matrix
	glPushMatrix();												// Push The Matrix
	glLoadIdentity();											// Reset The Matrix
}

void ViewPerspective()											// Set Up A Perspective View
{
	glMatrixMode( GL_PROJECTION );								// Select Projection
	glPopMatrix();												// Pop The Matrix
	glMatrixMode( GL_MODELVIEW );								// Select Modelview
	glPopMatrix();												// Pop The Matrix
}

void DrawRadialBlur(int times, float inc, int height, int width, GLfloat r, GLfloat g, GLfloat b, GLuint BLURTEXTURE)
{
	float spost = 0.0f;			
	float alphainc = 0.9f / times;
	float alpha = 0.2f;
	int num;										

	// Disable AutoTexture Coordinates
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

    glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,BLURTEXTURE);
	ViewOrtho(height, width);

	alphainc = alpha / times;

	glBegin(GL_QUADS);	
		for (num = 0;num < times;num++)
		{
			glColor4f(r, g, b, alpha);	
			glTexCoord2f(0+spost,1-spost);						// Texture Coordinate	( 0, 1 )
			glVertex2f(0,0);									// First Vertex		(   0,   0 )

			glTexCoord2f(0+spost,0+spost);						// Texture Coordinate	( 0, 0 )
			glVertex2f(0,width);									// Second Vertex	(   0, 480 )

			glTexCoord2f(1-spost,0+spost);						// Texture Coordinate	( 1, 0 )
			glVertex2f(height,width);								// Third Vertex		( 640, 480 )

			glTexCoord2f(1-spost,1-spost);						// Texture Coordinate	( 1, 1 )
			glVertex2f(height,0);									// Fourth Vertex	( 640,   0 )

			spost += inc;										// Gradually Increase spost (Zooming Closer To Texture Center)
			alpha = alpha - alphainc;							// Gradually Decrease alpha (Gradually Fading Image Out)
		}
	glEnd();													// Done Drawing Quads

	ViewPerspective();											// Switch To A Perspective View

	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glDisable(GL_TEXTURE_2D);									// Disable 2D Texture Mapping
	glDisable(GL_BLEND);										// Disable Blending
	glBindTexture(GL_TEXTURE_2D,0);								// Unbind The Blur Texture
  
}

void putTransImage(int image, float x, float y, float z, float blend, char test) {
	glPushMatrix();

    if (!test) glDisable(GL_DEPTH_TEST);
		
    glTranslatef(x,y,z);						
    glColor4f(1.0f,1.0f,1.0f,blend);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, image);	
    glBegin(GL_QUADS);									
      glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.1f, -1.1f,  0.0f);	
      glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.1f, -1.1f,  0.0f);	
      glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.1f,  1.1f,  0.0f);	
      glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.1f,  1.1f,  0.0f);	
    glEnd();	
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glBlendFunc(GL_ONE, GL_ONE);

    if (!test) glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}


