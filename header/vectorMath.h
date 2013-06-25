/**
 * @file vectorMath.h
 *
 * @author take
 */

#ifndef VECTORMATH_H_
#define VECTORMATH_H_

#include "xorshift.h"
#include "root.h"
#include <mmintrin.h>  // SSE
#include <emmintrin.h> // SSE2
#include <pmmintrin.h> // SSE3

namespace VectorMath {

/**
 * @brief 3x3ÇÃçsóÒéÆ
 */
inline float det(
        float a11,float a12,float a13,
        float a21,float a22,float a23,
        float a31,float a32,float a33
        ){
    float a1=a11*(a22*a33-a23*a32);
    float a2=a12*(a23*a31-a21*a33);
    float a3=a13*(a21*a32-a22*a31);
    return a1+a2+a3;
}

inline void rotXYZ(Vector4D& v,float rx,float ry,float rz,Vector4D& ret){
    float x=v.x;
    float y=v.y;
    float z=v.z;
    float sinx=sin(rx);
    float siny=sin(ry);
    float sinz=sin(rz);
    float cosx=cos(rx);
    float cosy=cos(ry);
    float cosz=cos(rz);
    ret.x=cosy*cosz*x+(cosz*sinx*siny-cosx*sinz)*y+(cosx*cosz*siny+sinx*sinz)*z;
    ret.y=cosy*sinz*x+(cosx*cosz+sinx*siny*sinz)*y+(-cosz*sinx+cosx*siny*sinz)*z;
    ret.z=-siny*x+cosy*sinx*y+cosx*cosy*z;
}

inline float getNorm(Vector4D& v){
    return sqrt(v.x*v.x+v.y*v.y+v.z*v.z);
}

inline float distance(Vector4D& a,Vector4D& b){
    float dx=a.x-b.x;
    float dy=a.y-b.y;
    float dz=a.z-b.z;
    return sqrt(dx*dx+dy*dy+dz*dz);
}

inline float distance_simd(Vector4D& a,Vector4D& b){
    __m128 sa,sb;
    float dxyz[4];
    sa=_mm_loadu_ps(a.xyzw);
    sb=_mm_loadu_ps(b.xyzw);
    sa=_mm_sub_ps(sa,sb);
    sa=_mm_mul_ps(sa,sa);
    _mm_storeu_ps(dxyz,sa);
    return sqrt(dxyz[0]+dxyz[1]+dxyz[2]);
}

inline void normalize(Vector4D& v){
    float n=getNorm(v);
    v.x/=n;
    v.y/=n;
    v.z/=n;
}

inline void setRandomSphereVector(Vector4D& vec){
    float z=Xorshift::nextf()*2.0f-1.0f;
    vec.z=z;
    float phi=Xorshift::nextf()*2.0f*M_PI;
    vec.x=sqrt(1-z*z)*cos(phi);
    vec.y=sqrt(1-z*z)*sin(phi);
//    vec.x=Xorshift::nextf()*2.0f-1.0f;
//    vec.y=Xorshift::nextf()*2.0f-1.0f;
//    vec.z=Xorshift::nextf()*2.0f-1.0f;
//    normalize(vec);
}

inline float dot(Vector4D& a,Vector4D& b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

inline void cross(Vector4D& a,Vector4D& b,Vector4D& n){
    n.x=a.y*b.z-a.z*b.y;
    n.y=a.z*b.x-a.x*b.z;
    n.z=a.x*b.y-a.y*b.x;
}

inline void sub(Vector4D& a,Vector4D& b,Vector4D& diff){
    diff.x=a.x-b.x;
    diff.y=a.y-b.y;
    diff.z=a.z-b.z;
}

inline void reflectionVector(Vector4D& v,Vector4D& normal,Vector4D& r){
//    printf("%f %f\n",getNorm(v),getNorm(normal));
    float d=dot(v,normal);
    r.x=-v.x+normal.x*d*2.0f;
    r.y=-v.y+normal.y*d*2.0f;
    r.z=-v.z+normal.z*d*2.0f;
}
inline int refractionVector(Vector4D& v,Vector4D& normal,float n1,float n2,Vector4D& r){
//    printf("%f %f\n",getNorm(v),getNorm(normal));
    float d=dot(v,normal);
    Vector4D n;
    if(d<0.0f){
        d*=-1.0f;
        n.x=-normal.x;
        n.y=-normal.y;
        n.z=-normal.z;
        float tmp=n1;
        n1=n2;
        n2=tmp;
    }else{
        n.x=normal.x;
        n.y=normal.y;
        n.z=normal.z;
    }
    float n1n2=n1/n2;
    float a=1.0f-(n1n2*n1n2*(1.0f-d*d));
    if(a<0.0f){
        reflectionVector(v,n,r);
        return 1;
    }
    float sqrta=sqrt(a);
    r.x=-n1n2*(v.x-d*n.x)-n.x*sqrta;
    r.y=-n1n2*(v.y-d*n.y)-n.y*sqrta;
    r.z=-n1n2*(v.z-d*n.z)-n.z*sqrta;
//    printf("%f %f\n",getNorm(v),getNorm(r));
    normalize(r);
    return 0;
}

inline void initAABB(AABB3D& aabb){
    aabb.xmax=aabb.ymax=aabb.zmax=-(1.0e+30);
    aabb.xmin=aabb.ymin=aabb.zmin=(1.0e+30);
}

inline void putVtoAABB(Vector4D& v,AABB3D& aabb){
    if(aabb.xmax<v.x){
        aabb.xmax=v.x;
    }
    if(aabb.xmin>v.x){
        aabb.xmin=v.x;
    }
    if(aabb.ymax<v.y){
        aabb.ymax=v.y;
    }
    if(aabb.ymin>v.y){
        aabb.ymin=v.y;
    }
    if(aabb.zmax<v.z){
        aabb.zmax=v.z;
    }
    if(aabb.zmin>v.z){
        aabb.zmin=v.z;
    }
}

} // namespace VectorMath

#endif /* VECTORMATH_H_ */
