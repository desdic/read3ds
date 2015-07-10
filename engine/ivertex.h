#ifndef __VERTEX_H__
#define __VERTEX_H__

class ivector3;

class ivertex {
public:
    inline bool operator ==(const ivertex &v) const {
        return (x==v.x && y==v.y && z==v.z);
    }

    float x,y,z;
};

#endif
