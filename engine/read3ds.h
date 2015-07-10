#ifndef __READ3DS_H__
#define __READ3DS_H__

#include "scene.h"

namespace iengine {
    enum known_3ds_chunks_ {
        CHUNK_RGBF         = 0x0010, CHUNK_RGBB         = 0x0011,
        CHUNK_PRJ          = 0xC23D, CHUNK_MLI          = 0x3DAA,
        CHUNK_MAIN         = 0x4D4D, CHUNK_OBJMESH      = 0x3D3D,
        CHUNK_BKGCOLOR     = 0x1200, CHUNK_AMBCOLOR     = 0x2100,
        CHUNK_OBJBLOCK     = 0x4000, CHUNK_TRIMESH      = 0x4100,
        CHUNK_VERTLIST     = 0x4110, CHUNK_VERTFLAGS    = 0x4111,
        CHUNK_FACELIST     = 0x4120, CHUNK_FACEMAT      = 0x4130,
        CHUNK_MAPLIST      = 0x4140, CHUNK_SMOOLIST     = 0x4150,
        CHUNK_TRMATRIX     = 0x4160, CHUNK_MESHCOLOR    = 0x4165,
        CHUNK_TXTINFO      = 0x4170, CHUNK_LIGHT        = 0x4600,
        CHUNK_SPOTLIGHT    = 0x4610, CHUNK_CAMERA       = 0x4700,
        CHUNK_HIERARCHY    = 0x4F00, CHUNK_VIEWPORT     = 0x7001,
        CHUNK_MATERIAL     = 0xAFFF, CHUNK_MATNAME      = 0xA000,
        CHUNK_AMBIENT      = 0xA010, CHUNK_DIFFUSE      = 0xA020,
        CHUNK_SPECULAR     = 0xA030, CHUNK_TEXTURE      = 0xA200,
        CHUNK_BUMPMAP      = 0xA230, CHUNK_REFLECTION   = 0xA220,
        CHUNK_MAPFILE      = 0xA300, CHUNK_MAPFLAGS     = 0xA351,
        CHUNK_MAPUSCALE    = 0xA354, CHUNK_MAPVSCALE    = 0xA356,
        CHUNK_MAPUOFFSET   = 0xA358, CHUNK_MAPVOFFSET   = 0xA35A,
        CHUNK_KEYFRAMER    = 0xB000, CHUNK_AMBIENTKEY   = 0xB001,
        CHUNK_TRACKINFO    = 0xB002, CHUNK_TRACKOBJNAME = 0xB010,
        CHUNK_TRACKPIVOT   = 0xB013, CHUNK_TRACKPOS     = 0xB020,
        CHUNK_TRACKROTATE  = 0xB021, CHUNK_TRACKSCALE   = 0xB022,
        CHUNK_TRACKMORPH   = 0xB026, CHUNK_TRACKHIDE    = 0xB029,
        CHUNK_OBJNUMBER    = 0xB030, CHUNK_TRACKCAMERA  = 0xB003,
        CHUNK_TRACKFOV     = 0xB023, CHUNK_TRACKROLL    = 0xB024,
        CHUNK_TRACKCAMTGT  = 0xB004, CHUNK_TRACKLIGHT   = 0xB005,
        CHUNK_TRACKLIGTGT  = 0xB006, CHUNK_TRACKSPOTL   = 0xB007,
        CHUNK_TRACKCOLOR   = 0xB025, CHUNK_FRAMES       = 0xB008,
        CHUNK_DUMMYNAME    = 0xB011, CHUNK_MAPROTANGLE  = 0xA35C,
        CHUNK_SHININESS    = 0xA040, CHUNK_SHINSTRENGTH = 0xA041,
        CHUNK_TRANSPARENCY = 0xA050, CHUNK_TRANSFALLOFF = 0xA052,
        CHUNK_REFBLUR      = 0xA053, CHUNK_SELFILLUM    = 0xA084,
        CHUNK_TWOSIDED     = 0xA081, CHUNK_TRANSADD     = 0xA083,
        CHUNK_WIREON       = 0xA085, CHUNK_SOFTEN       = 0xA08C,
        CHUNK_MATTYPE      = 0xA100, CHUNK_AMOUNTOF     = 0x0030,
        CHUNK_VERSION      = 0x0002, CHUNK_MASTERSCALE  = 0x0100
    };

/* Keep track of states */

    class Read3ds {
    public:
        Read3ds();
        ~Read3ds();
    
        int loadmesh3ds(const char *filename, Scene *scene);
        int loadkeyframer3ds(const char *filename, Scene *scene);

    private:

        char buffer[200];

        typedef struct { 
            unsigned short id;
            int  sub;         
            int  (Read3ds::*func) (FILE *f, Scene *scene);
        } chunkread_struct;
    
        typedef struct {
            unsigned short type;
            unsigned long size;
        } c_CHUNK;

        int read_NULL (FILE *fp, Scene *scene);
        int read_version (FILE *fp, Scene *scene);
        int read_vertlist(FILE *fp, Scene *scene);
        int read_facelist(FILE *fp, Scene *scene);
        int read_facemat(FILE *fp, Scene *scene);
        int read_maplist(FILE *fp, Scene *scene);
        int read_string(FILE *fp, Scene *scene);
        int read_mattype(FILE *fp, Scene *scene);
        int read_showheader(FILE *fp, Scene *scene);
        int read_rgbb(FILE *fp, Scene *scene);
        int read_amount(FILE *fp, Scene *scene);
        int read_camera(FILE *fp, Scene *scene);
        int read_light(FILE *fp, Scene *scene);
        int read_spotlight(FILE *fp, Scene *scene);

        int readbyte(FILE *fp);
        int readword(FILE *fp);
        void readstring(FILE *fp, char *buffer, int bufsize);
        int readdword(FILE *fp);
        float readfloat(FILE *fp);
        int readchunk(FILE *fp, c_CHUNK *chunk);
        int readchunks(FILE *fp, long p, chunkread_struct *chunks, int chunksize, unsigned short parent, Scene *scene);
        long filesize(FILE *fp);

    };    

}

#endif
