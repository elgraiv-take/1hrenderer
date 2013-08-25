/*
 * bvhraytracer.h
 *
 *  Created on: 2013/08/19
 *      Author: Take
 */

#ifndef BVHRAYTRACER_H_
#define BVHRAYTRACER_H_

#include "raytracer.h"

struct PolygonBVHNode{
    PolygonBVHNode* c0;
    PolygonBVHNode* c1;
    Triangle* triangles[4];
    int num;
    AABB3D bounds;
    PolygonBVHNode(){
        c0=c1=NULL;
        num=0;
    }
    ~PolygonBVHNode(){
        SAFE_DELETE_O(c0);
        SAFE_DELETE_O(c1);
    }
};

class BVHRayTracer:public IRayTracer{
private:
    Scene* scene;
    PolygonBVHNode* bvh;
public:
    BVHRayTracer(){
        bvh=NULL;
        scene=NULL;
    }
    ~BVHRayTracer(){
        SAFE_DELETE_O(bvh);
    }
    void setScene(Scene* _scene);
    void getIntersectionFromCamera(float x,float y,RayIntersection& intersection);
    void getIntersection(const RayObject& ray,RayIntersection& intersection);
};


#endif /* BVHRAYTRACER_H_ */
