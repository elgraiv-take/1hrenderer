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
    RayObject cpRay=ray;
    RayIntersection tempInt;
    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
    int n=ms->getSize();

    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct==cpRay.id){
                continue;
            }
            int res=ct->getIntersection(cpRay,tempInt);
            if(res){
                if(intersection.depth>tempInt.depth){
                    intersection=tempInt;
                    cpRay.maxDepth=tempInt.depth;
                }
            }
        }
    }
    ArrayList<IsHair*>* hs=scene->getIsHairList();
    n=hs->getSize();
    for(int i=0;i<n;i++){
        IsHair* hair=hs->get(i);
        int res=hair->getIntersection(cpRay,tempInt);
        if(res){
            if(intersection.depth>tempInt.depth){
//                printf("-----");
                intersection=tempInt;
                cpRay.maxDepth=tempInt.depth;
            }
        }
    }

}

