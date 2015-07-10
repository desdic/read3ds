#include <algorithm>
#include <vector>
#include <string.h>
#include <stdio.h>
#include "object.h"

using namespace std;

Object::Object() {
    name = NULL;
    vertices = NULL;
    faces = NULL;
    normals = NULL;
    uvs = NULL;
    shader = NULL;
    numverts = numfaces = numnormals = numuvs = 0;
    enablesmooth = false;
}

Object::~Object() {
    if (name!=NULL)
        delete [] name;

    if (vertices!=NULL)
        delete [] vertices;

    if (faces!=NULL)
        delete [] faces;

    if (normals!=NULL)
        delete [] normals;
    
    if (uvs!=NULL)
        delete [] uvs;
}

void Object::SetName(const char *n) {
    name = new char[strlen(n)+1];
    strncpy(name, n, strlen(n)+1);
    name[strlen(n)+1] = 0;
}

void Object::SetNumVerts(const unsigned short numv) {
    numverts = numv;
    vertices = new ivertex[numverts];
}

void Object::SetNumFaces(const unsigned short numf) {
    numfaces = numf;
    faces = new iface[numfaces];
}

void Object::SetNumUV(const unsigned short numu) {
    numuvs = numu;
    uvs = new iuv[numuvs];
}

void Object::SetPivot(const ivector3 &i) {
    pivot = i;
}
void Object::SetTranslate(const ivector3 &i) {
    translate = i;
}

void Object::SetScale(const ivector3 &i) {
    scale = i;
}

void Object::SetShader(Shader *s) {
    shader = s;
}

char *Object::GetName() const {
    return name;
}

unsigned short Object::GetNumVerts() const {
    return numverts;
}


unsigned short Object::GetNumFaces() const {
    return numfaces;
}


unsigned short Object::GetNumUV() const {
    return numuvs;
}

ivector3 Object::GetPivot() const {
    return pivot;
}

ivector3 Object::GetTranslate() const {
    return translate;
}

ivector3 Object::GetScale() const {
    return scale;
}

Shader *Object::GetShader() const {
    return shader;
}

void Object::CalcNormals() {
/*
    int i;
    ivector3 na,nb,nc,facenormal;

    if (normals==NULL) {
        normals = new ivector3[numverts];
    }

    // Clear
    for (i=0;i<numverts;i++) {
        normals[i].x = normals[i].y = normals[i].z = 0.0f;
    }
    
    // Calc face normal
    for (i=0;i<numfaces;i++) {
        na = vertices[ faces[i].a ];
        nb = vertices[ faces[i].b ];
        nc = vertices[ faces[i].c ];
        
        facenormal = (nb - na).crossproduct(nc - na);
        
        normals[ faces[i].a ]+=facenormal;
        normals[ faces[i].b ]+=facenormal;
        normals[ faces[i].c ]+=facenormal;
    }    
    
    // Average out vertex normals
    for (i=0;i<numverts;i++) {
            normals[i].normalize();
    }
*/

    int n,f;
    ivector3 na,nb,nc;

    if (normals==NULL) {
        normals = new ivector3[numverts];
    }

    for (f=0;f<numfaces;f++) {
        na = vertices[ faces[f].a ];
        nb = vertices[ faces[f].b ];
        nc = vertices[ faces[f].c ];
        
        faces[f].normal = (nb - na).crossproduct(nc - na);
        faces[f].normal.normalize();
    }

    for (n=0;n<numverts;n++) {
        
        ivector3 r(0.0f, 0.0f, 0.0f);
        int nbf=0;
        
        for (f=0;f<numfaces;f++) {
            if (faces[f].a==n || faces[f].b==n || faces[f].c==n) {
                r += faces[f].normal;
                nbf++;
            }
        }

        float inv = 1.0/(float)nbf;
        r = r*inv;
        r.normalize();
        normals[n] = r;
    }
}

void Object::SortFaces() {

    if (faces!=NULL) {

        sort(faces, faces+numfaces);

        unsigned short to = 0;
        unsigned short from = 0;
        int matid=faces[0].materialid;
        fromto stru;
        
        for (unsigned short i = 0; i<numfaces; i++) {
            if (matid!=faces[i].materialid) {
                to = i;
                stru.to = to;
                stru.from = from;
                stru.materialid = matid;
                //printf("a from: %d, to: %d, materialid: %d\n", stru.from, stru.to, stru.materialid);
                sorts.push_back(stru);
                from = i+1;
                matid = faces[i].materialid;
            }
        }
        stru.to = numfaces;
        stru.from = from;
        stru.materialid = faces[stru.from].materialid;
        //printf("b from: %d, from: %d, materialid: %d\n", stru.from, stru.to, stru.materialid);
        sorts.push_back(stru);
    }
}

void Object::CalcSmooth() {

    int i,j, f, nverts;

    // TODO: clean this up since we have vertex that is no longer in use

    int *list = new int[numverts];

    for (i=0;i<numverts;i++) {
        list[i] = i;
    }

    for (i=0;i<numverts;i++) {
        for (j=i+1;j<numverts;j++) {
       
            if (vertices[i] == vertices[j]) {
                // Mark vertex j for deletion
                list[j] = i;

                // Correct the face
                for (f=0;f<numfaces;f++) {
                    if (faces[f].a==j) {
                        faces[f].a = i;
                    } else
                    if (faces[f].b==j) {
                        faces[f].b = i;
                    } else
                    if (faces[f].c==j) {
                        faces[f].c = i;
                    }
                }
            }
        }
    }

    nverts = 0;
    for (i=0;i<numverts;i++) {
        if (list[i]==i) {
            nverts++;
        }
    }

    ivertex *newverts = new ivertex[nverts];

    int index = 0;
    for (i=0;i<numverts;i++) {
        if (list[i]==i) {
            newverts[index] = vertices[i];
            index++;
        } else {

            for (f=0;f<numfaces;f++) {
                if (faces[f].a == i) {
                    faces[f].a = index;
                }

                if (faces[f].b == i) {
                    faces[f].b = index;
                }

                if (faces[f].c == i) {
                    faces[f].b = index;
                }
            }
            
            //printf("Skipping vertex %d\n", i);

        }
    }

    if (numverts-nverts>0) {
        printf(" (Freed up %d vertexs) ", numverts-nverts);
    }

    delete [] vertices;
    delete [] list;
    vertices = newverts;
    numverts = nverts;
}

void Object::EnableSmooth() {
    enablesmooth = true;
}

void Object::Initialize() {

    if (enablesmooth==true) {
        printf("Smoothing %s   ", name);
        CalcSmooth();
        printf(" [DONE]\n");
    }
  
    SortFaces();
    CalcNormals();
}
