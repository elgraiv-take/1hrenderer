/**
 * @file raytracer.cpp
 *
 * @author take
 */


#include "raytracer.h"

void SimpleRayTracer::setScene(Scene* _scene){
    scene=_scene;
}

void SimpleRayTracer::getIntersectionFromCamera(float x,float y,RayIntersection& intersection){
    RayObject ray;
    scene->getCamera()->getRayObject(x,y,ray);
    ray.id=NULL;
    ray.ior=1.0f;
    getIntersection(ray,intersection);
}

void SimpleRayTracer::getIntersection(const RayObject& ray,RayIntersection& intersection){
    intersection.detect=0;
    intersection.depth=ray.maxDepth;
    intersection.ray=ray.direct;
    RayIntersection tempInt;
    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
    int n=ms->getSize();

    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct==ray.id){
                continue;
            }
            int res=ct->getIntersection(ray,tempInt);
            if(res){
                if(intersection.depth>tempInt.depth){
                    intersection=tempInt;
                }
            }
        }
    }

}

