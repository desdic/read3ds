#include <stdio.h>
#include <string.h>
#include "read3ds.h"

using namespace std;


// http://the-labs.com/Blender/3DS-details.html

namespace iengine {

    static unsigned short current_chunk=0;
    static unsigned short previos_chunk=0;
    static unsigned short last_chunk=0;

    Read3ds::Read3ds() {
    }
    
    Read3ds::~Read3ds() {
    }
    
    int Read3ds::readbyte(FILE *fp) {
        return (unsigned char)fgetc(fp);
    }

    int Read3ds::readword(FILE *fp) {
        char tmp[4];
        tmp[0] = readbyte(fp);
        tmp[1] = readbyte(fp);
        tmp[2] = 0;
        tmp[3] = 0;
        return ((int *)tmp)[0];
    }

    void Read3ds::readstring(FILE *fp, char *buffer, int bufsize) {
        int i=0;
        char c;
    
        while((c=readbyte(fp))!='\0' && i<bufsize-1) {
            buffer[i] = c;
            i++;
        }
        buffer[i]='\0';
    }

    int Read3ds::readdword(FILE *fp)
    {
        char    b[4];
        b[0]=readbyte(fp);
        b[1]=readbyte(fp);
        b[2]=readbyte(fp);
        b[3]=readbyte(fp);
        return ((int*)b)[0];
    }

    float Read3ds::readfloat(FILE *fp) {
        char b[4];

        b[0]=readbyte(fp);
        b[1]=readbyte(fp);
        b[2]=readbyte(fp);
        b[3]=readbyte(fp);

        return ((float*)b)[0];
    }

    int Read3ds::readchunk(FILE *fp, c_CHUNK *chunk) {
    
        chunk->type = readword(fp);
        chunk->size = readdword(fp);

        return ferror(fp);
    }

    int Read3ds::readchunks(FILE *fp, long p, chunkread_struct *chunks, int chunksize, unsigned short parent, Scene *scene) {

        c_CHUNK h;
        long where;
        int i, index;
        int error;

        last_chunk = parent;
        while ((where = ftell(fp)) < p ) 
        {
            if ((error = readchunk(fp, &h))!=0) return -1;

            //printf("Chunks: 0x%04X\n", h.type);

            current_chunk = h.type;
        
            index = -1;
            for (i = 0; i < chunksize; i++) {
                if (h.type == chunks[i].id) {
                    index = i;
                    break;
                }
            }

            if (index < 0) {
                fseek (fp, where + h.size, SEEK_SET);
            } else {
                where = where + h.size;

                // I know its ugly :( and doesen't work
                static Read3ds instance;

                if ((error = (instance.*chunks[index].func)(fp, scene))!=0)  return -1;

                if (chunks[index].sub)
                    if ((error = readchunks(fp, where, chunks, chunksize, h.type, scene)) !=0)
                        return -1;

                fseek (fp, where, SEEK_SET);
                previos_chunk = h.type;
            }

            if (ferror(fp)) return -1;

        }
        return 0;
    }

    int Read3ds::read_NULL (FILE *fp,Scene *scene) {
        return 0;
    }

    int Read3ds::read_version (FILE *fp, Scene *scene) {
        unsigned short version = readword(fp);

        if (version<3) {
            printf("This only supports version 3+\n");
            return -1;
        }
    
        return 0;
    }

    int Read3ds::read_mattype(FILE *fp, Scene *scene) {
        unsigned short mattype = readword(fp);

        vector<Material *>::iterator p = scene->Materials.end() -1 ;
        (*p)->SetShading( mattype );

        //printf("      0x%04X Mattype: %d\n", last_chunk, mattype);
        return 0;
    }

    int Read3ds::read_maplist(FILE *fp, Scene *scene) {
        unsigned short numuvs = readword(fp);
        float u,v;

        vector<Object *>::iterator p = scene->Objects.end()-1;
        (*p)->SetNumUV( numuvs );

        for (unsigned short i=0;i<numuvs;i++) {
            u = readfloat(fp);
            v = readfloat(fp);
            (*p)->uvs[i].u = u;
            (*p)->uvs[i].v = v;
            //printf("      %f, %f\n", u, v);
        }
        return 0;
    }

    int Read3ds::read_vertlist(FILE *fp, Scene *scene) {

        float x,y,z;

        unsigned short numpoints = readword(fp);
        Object *tmp = new Object;
  
        tmp->SetName( buffer );
        //printf("Object name: %s\n", buffer);
        tmp->SetNumVerts( numpoints );

        //printf("      Number of vertics: %d\n", numpoints);

        for (unsigned short i=0;i<numpoints;i++) {
            x = readfloat(fp);
            y = readfloat(fp);
            z = readfloat(fp);
            tmp->vertices[i].x = x;
            tmp->vertices[i].y = y;
            tmp->vertices[i].z = z;
            //printf("      %f, %f, %f\n", x,y,z);
        }

        scene->Objects.push_back( tmp );

        return 0;
    }

    int Read3ds::read_facemat(FILE *fp, Scene *scene) {

        //char buffer[200];
        readstring(fp, buffer, sizeof(buffer));

        int index = -1;

        for (vector<Material *>::iterator p=scene->Materials.begin(); p!=scene->Materials.end(); ++p) {
            index++;

            if (strcmp((*(p))->GetMaterialName(), buffer)==0) {
                break;
            }
        }

        if (index < 0) {
            printf("Error: Cannot find material %s\n", buffer);
            return -1;
        }

        //printf("Material id:%d\n", index);

        vector<Object *>::iterator p = scene->Objects.end()-1;

        unsigned short nfaces = readword(fp);

        for (unsigned short i=0;i<nfaces;i++) {
            unsigned short face = readword(fp);
        
            (*p)->faces[face].materialid = index;

            //printf("      face %d is material '%s'\n", face, buffer);
        }
        return 0;
    }

    int Read3ds::read_facelist(FILE *fp, Scene *scene) {
        unsigned short a, b, c, flags;
    
        unsigned short numfaces = readword(fp);
        //printf("      Number of faces:%d\n", numfaces);

        vector<Object *>::iterator p=scene->Objects.end()-1;
    
        (*p)->SetNumFaces( numfaces );

        for (unsigned short i=0;i<numfaces;i++) {
            a = readword(fp);
            b = readword(fp);
            c = readword(fp);
            flags = readword(fp);        
            //printf("      %d, %d, %d, 0x%04X\n", a, b, c, flags);
            (*p)->faces[i].a = a;
            (*p)->faces[i].b = b;
            (*p)->faces[i].c = c;
        }
        return 0;
    }

    int Read3ds::read_string(FILE *fp, Scene *scene) {

        memset(buffer, 0, sizeof(buffer));
        readstring(fp, buffer, sizeof(buffer));

        switch (current_chunk) {
        case CHUNK_MATNAME:
        {
            Material *tmp = new Material;

            tmp->SetMaterialName( buffer );
            scene->Materials.push_back( tmp );

            break;
        }
        case CHUNK_MAPFILE:
        {
            vector<Material *>::iterator p = scene->Materials.end()-1;
            (*p)->SetMapFilename( buffer );

            break;
        }
        case CHUNK_OBJBLOCK:
        {
            // Don't do anything
            // Use late allocation of objects since an object can be anything

            break;
        }
        default:
            printf("   You need to add 0x%04X to read_string\n", current_chunk);
        }

        return 0;
    }

    int Read3ds::read_light(FILE *fp, Scene *scene) {

        // Omni light

        float r,g,b;
        r = readfloat(fp);
        g = readfloat(fp);
        b = readfloat(fp);

        printf("Omni light %f %f %f\n", r,g,b);

        return 0;

    }

    int Read3ds::read_spotlight(FILE *fp, Scene *scene) {
    
        float tx,ty,tz,hotspot,roll;
    
        tx = readfloat(fp);
        ty = readfloat(fp);
        tz = readfloat(fp);
        hotspot = readfloat(fp);
        roll = readfloat(fp);    

        return 0;
    }

    int Read3ds::read_camera(FILE *fp, Scene *scene) {
        float px,py,pz;
        float tx,ty,tz;
        float bank_angle;
        float focus;

        Camera *tmp = new Camera;

        px = readfloat(fp);
        py = readfloat(fp);
        pz = readfloat(fp);

        tx = readfloat(fp);
        ty = readfloat(fp);
        tz = readfloat(fp);

        tmp->SetName( buffer );
        //printf("Camera name: %s\n", buffer);
        tmp->SetPos( ivector3(px,py,pz) );
        tmp->SetTarget( ivector3(tx,ty,tz) );

        //printf("      px:%f py:%f pz:%f tz:%f ty:%f tz:%f\n",px,py,pz,tx,ty,tz);

        bank_angle = readfloat(fp);
        focus = readfloat(fp);

        scene->Cameras.push_back( tmp );

        return 0;
    }

    int Read3ds::read_amount(FILE *fp, Scene *scene) {
        //unsigned short amount = readword(fp);

        //printf("      Amount %d\n", amount);
        return 0;
    }

    int Read3ds::read_showheader(FILE *fp, Scene *scene) {
/*    
      switch (current_chunk) {
      case CHUNK_MATERIAL:
      printf("\n\nMaterial entry\n");
      break;
      case CHUNK_AMBIENT:
      printf("   Material ambient\n");
      break;
      case CHUNK_DIFFUSE:
      printf("   Material diffuse\n");
      break;
      case CHUNK_SPECULAR:
      printf("   Material specular\n");
      break;
      case CHUNK_SHININESS:
      printf("   Shininess\n");
      break;
      case CHUNK_SHINSTRENGTH:
      printf("   Shine strength\n");
      break;
      case CHUNK_TRANSPARENCY:
      printf("   Transparentcy\n");
      break;
      case CHUNK_TRIMESH:
      printf("   Trimesh\n");
      break;

      default:
      printf("You need to add 0x%04X to read_showheader(...)\n", current_chunk);
      }
*/
        return 0;
    }

    int Read3ds::read_rgbb(FILE *fp, Scene *scene) {

        float r = (float)readbyte(fp)/255.0f;
        float g = (float)readbyte(fp)/255.0f;
        float b = (float)readbyte(fp)/255.0f;

        switch(last_chunk) {
        case CHUNK_LIGHT:
        {
            // TODO:
            // vector<Light *>::iterator p = Lights.end()-1;
            // (*p)->SetColor(r,g,b);
            break;
        }
        case CHUNK_AMBIENT:
        {
            vector<Material *>::iterator p=scene->Materials.end()-1;
            (*p)->SetAmbient(r, g, b);
            break;
        }
        case CHUNK_DIFFUSE:
        {
            vector<Material *>::iterator p=scene->Materials.end()-1;
            (*p)->SetDiffuse(r, g, b);
            break;
        }
        case CHUNK_SPECULAR:
        {
            vector<Material *>::iterator p=scene->Materials.end()-1;
            (*p)->SetSpecular(r, g, b);
            break;
        }
        }

        //printf("      Red: %f, Green: %f, Blue: %f\n", r, g, b);

        return 0;
    }

    long Read3ds::filesize(FILE *fp) {
        long length;
        if (fp) {
            fseek(fp, 0, SEEK_END);
            length = ftell(fp);
            fseek(fp, 0, SEEK_SET);
            return length;
        }
        return -1;
    }

    int Read3ds::loadmesh3ds(const char *filename, Scene *scene) {

        printf("Loading mesh from %s", filename);

        FILE *fp = fopen(filename, "rb");

        if (fp) {

            chunkread_struct world_chunks[] = { /* World definition chunks */
                {CHUNK_VERSION,      0, &Read3ds::read_version},
                {CHUNK_MASTERSCALE,  0, &Read3ds::read_NULL},
                {CHUNK_RGBF,         0, &Read3ds::read_NULL},
                {CHUNK_RGBB,         0, &Read3ds::read_rgbb},
                {CHUNK_AMOUNTOF,     0, &Read3ds::read_amount},
                {CHUNK_PRJ,          1, &Read3ds::read_NULL},
                {CHUNK_MLI,          1, &Read3ds::read_NULL},
                {CHUNK_MAIN,         1, &Read3ds::read_NULL},
                {CHUNK_OBJMESH,      1, &Read3ds::read_NULL},
                {CHUNK_BKGCOLOR,     1, &Read3ds::read_NULL},
                {CHUNK_AMBCOLOR,     1, &Read3ds::read_NULL},
                {CHUNK_OBJBLOCK,     1, &Read3ds::read_string},
                {CHUNK_TRIMESH,      1, &Read3ds::read_showheader},
                {CHUNK_VERTLIST,     0, &Read3ds::read_vertlist},
                {CHUNK_VERTFLAGS,    0, &Read3ds::read_NULL},
                {CHUNK_FACELIST,     1, &Read3ds::read_facelist},
                {CHUNK_MESHCOLOR,    0, &Read3ds::read_NULL},
                {CHUNK_FACEMAT,      0, &Read3ds::read_facemat},
                {CHUNK_MAPLIST,      0, &Read3ds::read_maplist},
                {CHUNK_TXTINFO,      0, &Read3ds::read_NULL},
                {CHUNK_SMOOLIST,     0, &Read3ds::read_NULL},
                {CHUNK_TRMATRIX,     0, &Read3ds::read_NULL},
                {CHUNK_LIGHT,        1, &Read3ds::read_light},
                {CHUNK_SPOTLIGHT,    0, &Read3ds::read_spotlight},
                {CHUNK_CAMERA,       0, &Read3ds::read_camera},
                {CHUNK_HIERARCHY,    1, &Read3ds::read_NULL},
                {CHUNK_VIEWPORT,     0, &Read3ds::read_NULL},
                {CHUNK_MATERIAL,     1, &Read3ds::read_showheader},
                {CHUNK_MATNAME,      0, &Read3ds::read_string},
                {CHUNK_AMBIENT,      1, &Read3ds::read_showheader},
                {CHUNK_DIFFUSE,      1, &Read3ds::read_showheader},
                {CHUNK_SPECULAR,     1, &Read3ds::read_showheader},
                {CHUNK_TEXTURE,      1, &Read3ds::read_NULL},
                {CHUNK_BUMPMAP,      1, &Read3ds::read_NULL},
                {CHUNK_REFLECTION,   1, &Read3ds::read_NULL},
                {CHUNK_MAPFILE,      0, &Read3ds::read_string},
                {CHUNK_MAPFLAGS,     0, &Read3ds::read_NULL},
                {CHUNK_MAPUSCALE,    0, &Read3ds::read_NULL},
                {CHUNK_MAPVSCALE,    0, &Read3ds::read_NULL},
                {CHUNK_MAPUOFFSET,   0, &Read3ds::read_NULL},
                {CHUNK_MAPVOFFSET,   0, &Read3ds::read_NULL},
                {CHUNK_MAPROTANGLE,  0, &Read3ds::read_NULL},
                {CHUNK_SHININESS,    1, &Read3ds::read_showheader},
                {CHUNK_SHINSTRENGTH, 1, &Read3ds::read_showheader},
                {CHUNK_TRANSPARENCY, 1, &Read3ds::read_showheader},
                {CHUNK_TRANSFALLOFF, 1, &Read3ds::read_NULL},
                {CHUNK_REFBLUR,      1, &Read3ds::read_NULL},
                {CHUNK_SELFILLUM,    1, &Read3ds::read_NULL},
                {CHUNK_TWOSIDED,     0, &Read3ds::read_NULL},
                {CHUNK_TRANSADD,     0, &Read3ds::read_NULL},
                {CHUNK_WIREON,       0, &Read3ds::read_NULL},
                {CHUNK_SOFTEN,       0, &Read3ds::read_NULL},
                {CHUNK_MATTYPE,      0, &Read3ds::read_mattype}
            };

            if (readchunks(fp, filesize(fp), world_chunks, int(sizeof (world_chunks) / sizeof (world_chunks[0])), 0, scene)==0) {
                fclose(fp);
                printf("   [OK]\n");
                return 0;
            }
        }
        printf("   [ERROR]\n");
        return -1;
    }

    int Read3ds::loadkeyframer3ds(const char *filename, Scene *scene) {

        printf("Loading keyframer info from %s", filename);

        FILE *fp = fopen(filename, "rb");

        if (fp) {
            chunkread_struct keyframer_chunks[] = { 
                {CHUNK_MAIN,         1, &Read3ds::read_NULL},
                {CHUNK_VERSION,      0, &Read3ds::read_version},
                {CHUNK_KEYFRAMER,    1, &Read3ds::read_NULL},
                {CHUNK_AMBIENTKEY,   1, &Read3ds::read_NULL},
                {CHUNK_TRACKINFO,    1, &Read3ds::read_NULL},
                {CHUNK_FRAMES,       0, &Read3ds::read_NULL},
                {CHUNK_TRACKOBJNAME, 0, &Read3ds::read_NULL},
                {CHUNK_DUMMYNAME,    0, &Read3ds::read_NULL},
                {CHUNK_TRACKPIVOT,   0, &Read3ds::read_NULL},
                {CHUNK_TRACKPOS,     0, &Read3ds::read_NULL},
                {CHUNK_TRACKCOLOR,   0, &Read3ds::read_NULL},
                {CHUNK_TRACKROTATE,  0, &Read3ds::read_NULL},
                {CHUNK_TRACKSCALE,   0, &Read3ds::read_NULL},
                {CHUNK_TRACKMORPH,   0, &Read3ds::read_NULL},
                {CHUNK_TRACKHIDE,    0, &Read3ds::read_NULL},
                {CHUNK_OBJNUMBER,    0, &Read3ds::read_NULL},
                {CHUNK_TRACKCAMERA,  1, &Read3ds::read_NULL},
                {CHUNK_TRACKCAMTGT,  1, &Read3ds::read_NULL},
                {CHUNK_TRACKLIGHT,   1, &Read3ds::read_NULL},
                {CHUNK_TRACKLIGTGT,  1, &Read3ds::read_NULL},
                {CHUNK_TRACKSPOTL,   1, &Read3ds::read_NULL},
                {CHUNK_TRACKFOV,     0, &Read3ds::read_NULL},
                {CHUNK_TRACKROLL,    0, &Read3ds::read_NULL}
            };

            if (readchunks(fp, filesize(fp), keyframer_chunks, int(sizeof (keyframer_chunks) / sizeof (keyframer_chunks[0])), 0, scene)==0) {
                fclose(fp);
                printf("   [OK]\n");
                return 0;
            }
        }
        printf("   [ERROR]\n");
        return -1;
    }

}
/*

int main(int argc, char **argv) {

    
    //FILE *fp = fopen("m_chld4.3ds", "rb");
    FILE *fp = fopen("sdscene8.3ds", "rb");
    
    iengine::Read3ds tt;
    Scene scene;

    if (fp) {
        tt.loadmesh3ds(fp, &scene);

        scene.dump();

        tt.loadkeyframer3ds(fp, &scene);
        fclose(fp);
    }

    return 0;
}
*/


