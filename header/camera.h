/**
 * @file camera.h
 *
 * @author take
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "dataStructure.h"

class PersCamera:public IFCamera{
private:
    Vector4D pos;
    Vector4D ex;
    Vector4D ey;
    Vector4D ez;
    float scale;
    float minDepth;
    float maxDepth;

public:
    PersCamera();
    void setDepth(float min,float max);
    void setPosition(Vector4D& _pos);
    void setRotation(Vector4D& _rot);
    void setAOV(float theta);
    void getRayObject(float x,float y,RayObject& ray);
};

#endif /* CAMERA_H_ */
