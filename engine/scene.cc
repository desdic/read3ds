#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "scene.h"
#include "exception.h"

using namespace std;

Scene::Scene() {
    activecam = NULL;
}

Scene::~Scene() {
}

void Scene::SetActiveCam(const char *camname) {
    for (vector<Camera *>::iterator p=Cameras.begin();p!=Cameras.end();++p) {

        if (strcmp((*p)->GetName(), camname)==0) {
            activecam = *p;
            return;
        }
    }
}

void Scene::dump() {

    printf("Scene stats:\n");
    printf("------------------------------------------------\n");
    printf("  Camera(s):   %d\n", Cameras.size());
    printf("  Material(s): %d\n", Materials.size());
    printf("  Object(s):   %d\n", Objects.size());
    printf("  Light(s):    %d\n", 0);
    
    int i=0;
    printf("  Camera(s):\n");
    for (vector<Camera *>::iterator p=Cameras.begin();p!=Cameras.end();++p) {
        printf("   [%d] Camera name: %s\n", i++, (*p)->GetName());
    }

    i = 0;
    printf("  Material(s):\n");
    for (vector<Material *>::iterator p=Materials.begin();p!=Materials.end();++p) {
        printf("   [%d] Material name: %s (%s)\n", i++, (*p)->GetMaterialName(), (*p)->GetMapFilename());
    }

    i = 0;
    int scenevertex=0;
    int scenefaces=0;
    printf("  Objects(s):\n");
    for (vector<Object *>::iterator p=Objects.begin();p!=Objects.end();++p) {
        scenevertex+=(*p)->GetNumVerts();
        scenefaces+=(*p)->GetNumFaces();
        printf("   [%d] Object name: %s\n", i++, (*p)->GetName());
    }
    printf("   Vertex: %d, Faces: %d\n", scenevertex, scenefaces);
    printf("------------------------------------------------\n\n");
}

void Scene::InitScene(const Config &c) {
    vector<string> t;

    c.Section("shaders", t);

    for (vector<string>::iterator p=t.begin();p!=t.end(); ++p) {
        string entry = *p;

        string::size_type equalpos = entry.find_first_of("=");
        string::size_type slashpos = entry.find_first_of("/");
        
        if (equalpos == string::npos || slashpos == string::npos) {
            string tmp;
            tmp+= "Invalid shader entry: " + entry;
            throw EngineException(tmp);
        }

        string::size_type commapos = entry.find_last_of(",");

        string name = entry.substr(slashpos+1, equalpos-slashpos-1);
        string file = entry.substr(equalpos+1, commapos-equalpos-1);
        string type = entry.substr(commapos+1, entry.length());

        GLuint shadertype;    
        
        if (type=="GL_VERTEX_SHADER") {
            shadertype = GL_VERTEX_SHADER;
        } else if (type=="GL_FRAGMENT_SHADER"){
            shadertype = GL_FRAGMENT_SHADER;
        } else {
            string tmp;
            tmp+= "Unknown shader type: "+type;
            throw EngineException(tmp);
        }

        bool foundshader = false;
        
        if (name=="") {
            cout << "Warning! shader name is empty so it will default to the name unknown" << endl;
            name = "unknown";
        }

        for (vector<Shader *>::iterator s=Shaders.begin(); s!=Shaders.end(); ++s) {
            
            if ((*s)->GetName() == name) {
                foundshader = true;
                (*s)->LoadFromFile( file, shadertype );
                break;
            }
        }

        if (!foundshader) {
            Shader *t = new Shader(name);
            //cout << "Creating shader: " << name << endl;
            t->LoadFromFile( file, shadertype );
            Shaders.push_back(t);
        }

    }


    for (std::vector<Object *>::iterator p=Objects.begin();p!=Objects.end();++p) {

        string tmp;
        tmp = "object\""+string((*p)->GetName())+"\"";

        string lala = "smooting";

        if (c.Value( tmp, lala, bool(false))) {
            (*p)->EnableSmooth();
        }

        string shadername = c.Value( tmp, "shader", string("unknown"));
        if (shadername!="unknown") {

            for (vector<Shader *>::iterator sa=Shaders.begin();sa!=Shaders.end();++sa) {
                if ((*sa)->GetName()==shadername) {
                    (*p)->SetShader( *sa );
                    break;
                }
            }
        }

        (*p)->Initialize();
    }    

}
