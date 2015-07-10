// http://www.morrowland.com/apron/tut_gl.php
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <math.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "main.h"
#include "shader.h"
#include "read3ds.h"
#include "config.h"
#include "exception.h"

using namespace iengine;
using namespace std;

extern S_AppStatus AppStatus;

const int height = 640;
const int width = 480;

const char *requiredextensions[] = {"GL_ARB_fragment_shader",
                                    "GL_ARB_vertex_shader",
                                    "GL_ARB_shader_objects",
                                    "GL_ARB_multitexture",
                                    "GL_EXT_texture_env_combine",
                                    0};

#ifndef PI
#define PI		3.1415926535897932384626433832795
#endif

Texture tex;
Scene scene;
GLuint texture;
//Shader shader;
bool light = true;

Config c("config.ini");

float anglex, angley, anglez;

GLfloat LightAmbient[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat LightDiffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.0f };
//GLfloat LightPosition[] = { 100.0f, 100.0f, 100.0f, 1.0f };

void handleglerros() {
    GLenum errcode;
    const GLubyte *errstring;
    if ((errcode = glGetError())!=GL_NO_ERROR) {
        errstring = gluErrorString(errcode);
        fprintf(stderr, "OpenGL error: %s\n", errstring);
        exit(1);
    }
}

bool InitGL(SDL_Surface *S) {

	glewInit();
   
    int i=0;
    while (requiredextensions[i]!=0) {

        if (GL_TRUE != glewGetExtension(requiredextensions[i]))        
        {
            printf("%s is not supported\n", requiredextensions[i]);
            return false;
        }
        i++;
    }

    


/*
    shader.LoadFromFile("shaders/toon.vert", GL_VERTEX_SHADER);
    shader.LoadFromFile("shaders/toon.frag", GL_FRAGMENT_SHADER);
    shader.LoadFromFile("shaders/toon2.frag", GL_FRAGMENT_SHADER);


	float colors[12] = {0.4,0.4,0.8,1.0,
						0.2,0.2,0.4,1.0,
						0.1,0.1,0.1,1.0};

	float specIntensity = 0.98;
	float sc[4] = {0.8,0.8,0.8,1.0};
	float threshold[2] = {0.5,0.25};


    shader.setUniform1f("specIntensity", specIntensity);
    shader.setUniform4f("specColor", sc[0], sc[1], sc[2], sc[3]);
    shader.setUniform1fv("t", 2, threshold);
    shader.setUniform4fv("colors", 3, colors);

    shader.LoadFromFile("shaders/pointlight.vert", GL_VERTEX_SHADER);
    shader.LoadFromFile("shaders/pointlight.frag", GL_FRAGMENT_SHADER);


    shader.LoadFromFile("shaders/textureComb.vert", GL_VERTEX_SHADER);
    shader.LoadFromFile("shaders/textureComb.frag", GL_FRAGMENT_SHADER);

    shader.setUniform3fv("lightPos", 3, LightPosition);
*/

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glViewport(0,0,height,width);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();				// Reset The Projection Matrix
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,5.0f,10000.0f);


	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Texture mapping perspective correction (OpenGL... thank you so much!)

    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL); // Polygon rasterization mode (polygon filled)
	glEnable(GL_CULL_FACE); // Enable the back face culling
    glEnable(GL_DEPTH_TEST); // Enable the depth test (also called z buffer)

    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH); // Type of shading for the polygons
    glDepthFunc(GL_LEQUAL);

    //glShadeModel(GL_FLAT); 
    //glEnable(GL_LIGHT1);

/*
    const GLfloat am[4] = {0.0f, 0.0f, 0.0f, 0.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, am);

    glEnable ( GL_COLOR_MATERIAL ) ;    
    glColorMaterial ( GL_FRONT_AND_BACK,  GL_AMBIENT_AND_DIFFUSE ) ;
*/

    // Adding light after rotation so it follows the eye
        
    /* Setup The Ambient Light */
    glLightfv( GL_LIGHT1, GL_AMBIENT, LightAmbient );
    
    /* Setup The Diffuse Light */
    glLightfv( GL_LIGHT1, GL_DIFFUSE, LightDiffuse );
        
    /* Position The Light */
    glLightfv( GL_LIGHT1, GL_POSITION, LightPosition );
    
    /* Enable Light One */
    glDisable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );

    //glEnable ( GL_COLOR_MATERIAL ) ;    
    //glColorMaterial ( GL_FRONT_AND_BACK,  GL_AMBIENT_AND_DIFFUSE ) ;

    return true;
}


bool Initialize(void)
{
	AppStatus.Visible		= true;	
	AppStatus.MouseFocus	= true;	
	AppStatus.KeyboardFocus = true;

    Read3ds loader;

   loader.loadmesh3ds("m_chld4.3ds", &scene);
   //loader.loadmesh3ds("bart.3ds", &scene);

    scene.InitScene(c);

    //loader.loadmesh3ds("m_man3.3ds", &scene);
    //loader.loadmesh3ds("test.3ds", &scene);
    
    //scene.dump();
    //loader.loadmesh3ds("spaceship.3ds", &scene);
    
/*
    scene.SetActiveCam("Camera01");

    ivector3 campos, targetpos;
    campos = scene.activecam->GetPos();
    targetpos = scene.activecam->GetTarget();

    gluLookAt(campos.x, campos.y, campos.z, 
              targetpos.x ,targetpos.y, targetpos.z,
              0.0f,1.0f,0.0f);
*/

	return true;											   
}



void Update(Uint32 Milliseconds, Uint8 *Keys)					
{
	if(Keys)													
	{

		if(Keys[SDLK_ESCAPE])									
		{
			TerminateApplication();								
		}

		if(Keys[SDLK_F1])										
		{
			ToggleFullscreen();									
		}

        if (Keys[SDLK_l]) {
            light = !light;
            if ( !light )
                glDisable( GL_LIGHTING );
            else
                glEnable( GL_LIGHTING );
        }
        
        if (Keys[SDLK_UP]) {
            anglex+=0.05;
        }
        if (Keys[SDLK_DOWN]) {
            anglex-=0.05;
        }
        if (Keys[SDLK_LEFT]) {
            angley-=0.05;
        }
        if (Keys[SDLK_RIGHT]) {
            angley+=0.05;
        }
	}

    if (anglex>359) anglex=0.0f;
    if (angley>359) angley=0.0f;
    if (anglez>359) anglez=0.0f;

}


// Draw the scene
void Draw (void)												
{
    // Fps
    static GLint T0     = 0;
    static GLint Frames = 0;

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
    glMatrixMode(GL_MODELVIEW); // Modeling transformation

	glLoadIdentity();											
  

    gluLookAt(0.0f, 0.0f, 300.0f, 
              0.0f ,0.0f, 0.0f,
              0.0f,1.0f,0.0f);


    glRotatef(270.0,1.0,0.0,0.0);

    for (std::vector<Object *>::iterator p=scene.Objects.begin();p!=scene.Objects.end();++p) {

        glPushMatrix();

        glRotatef(anglex,1.0,0.0,0.0);
        glRotatef(angley,0.0,1.0,0.0);
        glRotatef(anglez,0.0,0.0,1.0);

        for (std::vector<fromto>::iterator t=(*p)->sorts.begin();t!=(*p)->sorts.end();++t) {
            unsigned short from = t->from;
            unsigned short to = t->to;
            int materialid = t->materialid;
            bool turntextureoff = false;

            if (materialid>=0) {

                int textureid = scene.Materials[ materialid ]->GetTextureID();

                if (textureid>0) {
                    turntextureoff = true;
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, textureid);
                    if ((*p)->GetShader()!=NULL) {
                        (*p)->GetShader()->Begin();
                    }
                }

                glMateriali(GL_FRONT_AND_BACK, GL_SHININESS, 8); //128
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,  scene.Materials[ materialid ]->GetAmbient());
                glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, scene.Materials[ materialid ]->GetSpecular());
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  scene.Materials[ materialid ]->GetDiffuse());                

            }

            for (unsigned short i=from; i<to; i++) {
                glBegin(GL_TRIANGLES);
            
                // Face a
                glNormal3f( (*p)->normals[ (*p)->faces[i].a ].x, 
                            (*p)->normals[ (*p)->faces[i].a ].y,
                            (*p)->normals[ (*p)->faces[i].a ].z);
            
                glTexCoord2f( (*p)->uvs[ (*p)->faces[i].a ].u,
                              (*p)->uvs[ (*p)->faces[i].a ].v);
                
                glVertex3f( (*p)->vertices[ (*p)->faces[i].a ].x, 
                            (*p)->vertices[ (*p)->faces[i].a ].y,
                            (*p)->vertices[ (*p)->faces[i].a ].z);

                // Face b
                glNormal3f( (*p)->normals[ (*p)->faces[i].b ].x, 
                            (*p)->normals[ (*p)->faces[i].b ].y,
                            (*p)->normals[ (*p)->faces[i].b ].z);
                
                glTexCoord2f( (*p)->uvs[ (*p)->faces[i].b ].u,
                              (*p)->uvs[ (*p)->faces[i].b ].v);
                
                glVertex3f( (*p)->vertices[ (*p)->faces[i].b ].x, 
                            (*p)->vertices[ (*p)->faces[i].b ].y,
                            (*p)->vertices[ (*p)->faces[i].b ].z);
                
                // Face c
                glNormal3f( (*p)->normals[ (*p)->faces[i].c ].x, 
                            (*p)->normals[ (*p)->faces[i].c ].y,
                            (*p)->normals[ (*p)->faces[i].c ].z);
                
                glTexCoord2f( (*p)->uvs[ (*p)->faces[i].c ].u,
                              (*p)->uvs[ (*p)->faces[i].c ].v);
                
                glVertex3f( (*p)->vertices[ (*p)->faces[i].c ].x, 
                            (*p)->vertices[ (*p)->faces[i].c ].y,
                            (*p)->vertices[ (*p)->faces[i].c ].z);
                
                glEnd();                
            }
            
            if (turntextureoff) {
                if ((*p)->GetShader()!=NULL) {
                    (*p)->GetShader()->End();
                }
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
            }
        }

        glPopMatrix();

    }


	glFlush ();													

    Frames++;
    {
        GLint t = SDL_GetTicks();
        if (t - T0 >= 5000) {
            GLfloat seconds = (t - T0) / 1000.0;
            GLfloat fps = Frames / seconds;
            printf("%d frames in %g seconds = %g FPS\n", Frames, seconds, fps);

            T0 = t;
            Frames = 0;
        }
    }

//    anglex-=0.05;
//    angley-=0.05;
    anglez-=0.05;
}

// Cleanup
void Deinitialize (void)									 
{
    
}
