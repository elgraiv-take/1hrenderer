/*
 * rayUtil.h
 *
 *  Created on: 2013/08/16
 *      Author: take
 */

#ifndef RAYUTIL_H_
#define RAYUTIL_H_

#include "structDef.h"

namespace RaytraceUtil {

inline int linearFunc(float orgx,float dx,float tgtx,float orgy1,float dy1,float miny1,float maxy1,float orgy2,float dy2,float miny2,float maxy2,float* tret){
    if(dx==0.0f){
        return 0;
    }
    float x=tgtx-orgx;
    float a1=dy1/dx;
    float y1=(a1*x)+orgy1;
    if(y1<miny1||y1>maxy1){
        return 0;
    }

    float a2=dy2/dx;
    float y2=(a2*x)+orgy2;
    if(y2<miny2||y2>maxy2){
        return 0;
    }
    (*tret)=x/dx;
    return 1;
}

inline int checkHit(const AABB3D& aabb,const RayObject& ray){
    float t;
    if(linearFunc(ray.start.x,ray.direct.x,aabb.xmax,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t)){
        return 1;
    }
    if(linearFunc(ray.start.x,ray.direct.x,aabb.xmin,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t)){
        return 1;
    }
    if(linearFunc(ray.start.y,ray.direct.y,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t)){
        return 1;
    }
    if(linearFunc(ray.start.y,ray.direct.y,aabb.ymin,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t)){
        return 1;
    }
    if(linearFunc(ray.start.z,ray.direct.z,aabb.zmax,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,&t)){
        return 1;
    }
    if(linearFunc(ray.start.z,ray.direct.z,aabb.zmin,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,&t)){
        return 1;
    }
    return 0;
}

inline int checkRayRange(const AABB3D& aabb,const RayObject& ray,float* min,float* max){
    float t;
    float tmax=0.0f;
    float tmin=FLOAT_MAX_VAL;
    int c,c1=0;
    c=linearFunc(ray.start.x,ray.direct.x,aabb.xmax,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    c=linearFunc(ray.start.x,ray.direct.x,aabb.xmin,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    c=linearFunc(ray.start.y,ray.direct.y,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    c=linearFunc(ray.start.y,ray.direct.y,aabb.ymin,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,ray.start.z,ray.direct.z,aabb.zmin,aabb.zmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    c=linearFunc(ray.start.z,ray.direct.z,aabb.zmax,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    c=linearFunc(ray.start.z,ray.direct.z,aabb.zmin,ray.start.y,ray.direct.y,aabb.ymin,aabb.ymax,ray.start.x,ray.direct.x,aabb.xmin,aabb.xmax,&t);
    if(c){
        tmax=MAX(tmax,t);
        tmin=MIN(tmin,t);
        c1++;
    }
    if(c1>1){
        (*max)=tmax;
        (*min)=tmin;
        return 1;
    }
    return 0;
}

} // namespace RaytraceUtil

#endif /* RAYUTIL_H_ */
