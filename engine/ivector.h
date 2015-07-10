#ifndef __IVECTOR_H__
#define __IVECTOR_H__

#include <math.h>
#include "ivertex.h"

//class ivertex;

class ivector3 {
public:
    ivector3() { x=y=z=0.0f;}
    ivector3(float x_, float y_, float z_) {x=x_; y=y_; z=z_;}
    ivector3(const ivector3 &v) { x=v.x; y=v.y; z=v.z;}

    ivector3(const ivertex &v) { x=v.x; y=v.y; z=v.z;}

    inline bool operator ==(const ivector3 &v) const {
        return (x==v.x && y==v.y && z==v.z);
    }

	inline ivector3 operator+(const ivector3 &vVector) const {return ivector3(vVector.x+x, vVector.y+y, vVector.z+z);}

	inline ivector3 operator-(const ivector3 &vVector) const {return ivector3(x-vVector.x, y-vVector.y, z-vVector.z);}
    inline ivector3 operator-() const { return ivector3(-x, -y, -z);}

    inline ivector3 operator/ (float s) const { float f = 1.0f/s; return ivector3(x*f, y*f, z*f); }
    inline ivector3 operator/(const ivector3 &v) const { return ivector3(x/v.x, y/v.y, z/v.z); }

	inline ivector3 operator*(float number) const {return ivector3(x*number, y*number, z*number);}
    inline ivector3 operator*(const ivector3 &v) const { return ivector3(x*v.x, y*v.y, z*v.z); }
    
    inline void operator+= (const ivector3 &v) { x+=v.x; y+=v.y; z+=v.z; }
    inline void operator-= (const ivector3 &v) { x-=v.x; y-=v.y; z-=v.z; }
    inline void operator/= (const int number) { x/=number; y/=number; z/=number;}

    inline float operator[] (int i) {
        if (i==0)
            return x;
        else if(i==1)
            return y;
        else return z;
    }

    inline float dot( const ivector3 &v) const { return v.x*x + v.y*y + v.z*z; }
    inline ivector3 crossproduct( const ivector3 &v) const { return ivector3( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x ); }

    inline float magnitude() { return (float)(sqrtf((x*x)+(y*y)+(z*z))); }

    inline float distance( const ivector3 &v) const { return ( *this - v ).magnitude(); }
    
    inline void normalize() {
        float f = (x*x + y*y + z*z);
        if (f==0.0f)
            return;

        float l = 1.0f/sqrtf(f);
        x *= l;
        y *= l;
        z *= l;            

    }

    float x,y,z;
};


#endif
