/**
 * @file camera.cpp
 *
 * @author take
 */

#include "camera.h"
#include "vectorMath.h"

PersCamera::PersCamera(){
    ex.y=ex.z=ey.x=ey.z=ez.x=ez.y=0.0f;
    ex.x=ey.y=ez.z=1.0f;
    minDepth=0.0f;
    maxDepth=1.0f;
    scale=1.0f;
}

void PersCamera::setDepth(float min,float max){
    minDepth=min;
    maxDepth=max;
}

void PersCamera::setPosition(Vector4D& _pos){
    pos=_pos;
}
void PersCamera::setRotation(Vector4D& _rot){
    Vector4D tmpv(1.0f,0.0f,0.0f);
    VectorMath::rotXYZ(tmpv,_rot.x,_rot.y,_rot.z,ex);
    tmpv.x=0.0f;
    tmpv.y=-1.0f;
    tmpv.z=0.0f;
    VectorMath::rotXYZ(tmpv,_rot.x,_rot.y,_rot.z,ey);
    tmpv.x=0.0f;
    tmpv.y=0.0f;
    tmpv.z=-1.0f;
    VectorMath::rotXYZ(tmpv,_rot.x,_rot.y,_rot.z,ez);
}

void PersCamera::setAOV(float theta){
    float tanTheta=tanf(theta/2.0f);
    scale=tanTheta;
}

void PersCamera::getRayObject(float x,float y,RayObject& ray){
    ray.start=pos;
    ray.maxDepth=maxDepth;
    ray.minDepth=minDepth;

    ray.direct.x=scale*x*ex.x+scale*y*ey.x+ez.x;
    ray.direct.y=scale*x*ex.y+scale*y*ey.y+ez.y;
    ray.direct.z=scale*x*ex.z+scale*y*ey.z+ez.z;
    VectorMath::normalize(ray.direct);

}
