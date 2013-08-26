/**
 * @file renderer.h
 *
 * @author take
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include "root.h"
#include "dataStructure.h"
#include "raytracer.h"
#include "bvhraytracer.h"

class IRenderer{
public:
    virtual ~IRenderer(){}
    virtual void render(Scene* scene,RawImage& img)=0;
};

class SimpleRenderer:public IRenderer{
private:
    BVHRayTracer tracer;
    Scene* currentScene;
    float ambientOcclusion(RayIntersection& intersection);
    void normal(RayIntersection& intersection,ColorRGBA& ret);
    void tangent(RayIntersection& intersection,ColorRGBA& ret);
    float simpleDeffuse(RayIntersection& intersection);
    void simplePathTraceSub(RayIntersection& intersection,ColorRGBA& c,int depth);
    void simplePathTrace(RayIntersection& intersection,ColorRGBA& c);
public:
    void render(Scene* scene,RawImage& img);
};



#endif /* RENDERER_H_ */
