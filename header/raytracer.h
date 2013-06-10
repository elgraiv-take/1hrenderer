/**
 * @file raytracer.h
 *
 * @author take
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "dataStructure.h"

class SimpleRayTracer{
private:
    Scene* scene;
public:
    void setScene(Scene* _scene);
    void getIntersectionFromCamera(float x,float y,RayIntersection& intersection);
    void getIntersection(const RayObject& ray,RayIntersection& intersection);
};


#endif /* RAYTRACER_H_ */
