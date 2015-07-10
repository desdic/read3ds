#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <vector>
#include "ivector.h"
#include "mapping.h"
#include "structs.h"
#include "shader.h"

struct fromto {
    unsigned short from;
    unsigned short to;
    int materialid;
};

class Object {
public:
    Object();
    ~Object();

    void Initialize();

    void SetName(const char *n);
    void SetNumVerts(const unsigned short numv);
    void SetNumFaces(const unsigned short numf);
    void SetNumUV(const unsigned short numu);
    void SetPivot(const ivector3 &i);
    void SetTranslate(const ivector3 &i);
    void SetScale(const ivector3 &i);
    void SetShader(Shader *s);

    void EnableSmooth();

    char *GetName() const;
    unsigned short GetNumVerts() const;
    unsigned short GetNumFaces() const;
    unsigned short GetNumUV() const;
    ivector3 GetPivot() const;
    ivector3 GetTranslate() const;
    ivector3 GetScale() const;
    Shader *GetShader() const;

    void CalcNormals();
    void SortFaces();
    void CalcSmooth();

private:
    char *name;

    unsigned short numverts;
    unsigned short numfaces;
    unsigned short numnormals;
    unsigned short numuvs;

    ivector3 pivot;
    ivector3 translate;
    ivector3 scale;

    bool enablesmooth;

    Shader *shader;
    
    // imatrix3x4 keyframermatrix;
    // iquad rotate;
public:
    ivertex *vertices;
    iface *faces;
    ivector3 *normals;
    iuv *uvs;
    std::vector<fromto> sorts;
};

#endif
