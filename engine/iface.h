#ifndef __IFACE_H__
#define __IFACE_H__

#include "ivector.h"

class iface {
public:

    inline int operator<(const iface &a) const {
        if (this->materialid < a.materialid) return 1;
        return 0;
    }

    inline bool operator ==(const iface &v) const {
        return (a==v.a && b==v.b && c==v.c);
    }

    unsigned short a,b,c;
    int materialid;
    ivector3 normal;
};


#endif
