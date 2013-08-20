/**
 * @file raytracer.cpp
 *
 * @author take
 */

#include "bvhraytracer.h"
#include "rayUtil.h"

namespace PolygonBVHUtil {

void heapDown(Triangle** tris,int index,int size,int axis){
    int lc=index*2+1;

    if(!(lc<size)){
        return;
    }

    float max=tris[index]->aabb.center.xyzw[axis];
    int nindex=index;
    if(max<tris[lc]->aabb.center.xyzw[axis]){
        max=tris[lc]->aabb.center.xyzw[axis];
        nindex=lc;
    }
    int rc=index*2+2;
    if((rc<size)&&(max<tris[rc]->aabb.center.xyzw[axis])){
        nindex=rc;
    }

    if(nindex!=index){
        Triangle* tmp=tris[index];
        tris[index]=tris[nindex];
        tris[nindex]=tmp;
        heapDown(tris,nindex,size,axis);
    }
}
void heapUp(Triangle** tris,int index,int axis){
    if(index==0){
        return;
    }
    int pindex=(index-1)/2;
    if(tris[index]->aabb.center.xyzw[axis]>tris[pindex]->aabb.center.xyzw[axis]){
        Triangle* tmp=tris[index];
        tris[index]=tris[pindex];
        tris[pindex]=tmp;
        heapUp(tris,pindex,axis);
    }
}

void sortBone(Triangle** tris,int size,int axis){
    for(int i=1;i<size;i++){
        heapUp(tris,i,axis);
    }
    for(int i=size-1;i>0;i--){
        Triangle* tmp=tris[i];
        tris[i]=tris[0];
        tris[0]=tmp;
        heapDown(tris,0,i,axis);
    }
}

PolygonBVHNode* createBVHR(Triangle** tris,int offset,int size,int axis){
    PolygonBVHNode* ret=new PolygonBVHNode();
    if(size<5){
        ret->num=size;
        ret->triangles[0]=tris[offset];
        ret->bounds=tris[offset]->aabb;
        for(int i=1;i<size;i++){
            ret->triangles[i]=tris[offset+i];
            ret->bounds.xmax=MAX(ret->bounds.xmax,tris[offset+i]->aabb.xmax);
            ret->bounds.ymax=MAX(ret->bounds.ymax,tris[offset+i]->aabb.ymax);
            ret->bounds.zmax=MAX(ret->bounds.zmax,tris[offset+i]->aabb.zmax);
            ret->bounds.xmin=MIN(ret->bounds.xmin,tris[offset+i]->aabb.xmin);
            ret->bounds.ymin=MIN(ret->bounds.ymin,tris[offset+i]->aabb.ymin);
            ret->bounds.zmin=MIN(ret->bounds.zmin,tris[offset+i]->aabb.zmin);
        }
        ret->bounds.center.x=(ret->bounds.xmax+ret->bounds.xmin)*0.5f;
        ret->bounds.center.y=(ret->bounds.ymax+ret->bounds.ymin)*0.5f;
        ret->bounds.center.z=(ret->bounds.zmax+ret->bounds.zmin)*0.5f;
        return ret;
    }
    sortBone(tris+offset,size,axis);
    int naxis=(axis+1)%3;
    int lsize=size/2;
    int rsize=size-lsize;
    ret->c0=createBVHR(tris,offset,lsize,naxis);
    ret->c1=createBVHR(tris,offset+lsize,rsize,naxis);
    ret->bounds.xmax=MAX(ret->c0->bounds.xmax,ret->c1->bounds.xmax);
    ret->bounds.ymax=MAX(ret->c0->bounds.ymax,ret->c1->bounds.ymax);
    ret->bounds.zmax=MAX(ret->c0->bounds.zmax,ret->c1->bounds.zmax);
    ret->bounds.xmin=MIN(ret->c0->bounds.xmin,ret->c1->bounds.xmin);
    ret->bounds.ymin=MIN(ret->c0->bounds.ymin,ret->c1->bounds.ymin);
    ret->bounds.zmin=MIN(ret->c0->bounds.zmin,ret->c1->bounds.zmin);
    ret->bounds.center.x=(ret->bounds.xmax+ret->bounds.xmin)*0.5f;
    ret->bounds.center.y=(ret->bounds.ymax+ret->bounds.ymin)*0.5f;
    ret->bounds.center.z=(ret->bounds.zmax+ret->bounds.zmin)*0.5f;

    return ret;
}
void traverseSub(const PolygonBVHNode* bvh,RayObject& ray,RayIntersection& intersection,RayIntersection& temp){

    if(!RaytraceUtil::checkHit(bvh->bounds,ray)){
        return;
    }

    if(!bvh->num){
        traverseSub(bvh->c0,ray,intersection,temp);
        traverseSub(bvh->c1,ray,intersection,temp);
    }else{

        for(int i=0;i<bvh->num;i++){
//            printf("---");
            if(ray.id==bvh->triangles[i]){

                continue;
            }

            int res=bvh->triangles[i]->getIntersection(ray,temp);
//            printf("%d",res);
            if(res){

                if(intersection.depth>temp.depth){
                    intersection=temp;

                    ray.maxDepth=temp.depth;
                }
            }
        }
    }
}

void traverse(const PolygonBVHNode* bvh,RayObject& ray,RayIntersection& intersection){
    RayIntersection temp;
    traverseSub(bvh,ray,intersection,temp);
}

} // namespace IsHairBVHUtil

void BVHRayTracer::setScene(Scene* _scene){
    scene=_scene;

    int trinum=0;
    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
    int n=ms->getSize();
    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        trinum+=m;
    }
    Triangle** tris=new Triangle*[trinum];
    for(int i=0,c=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++,c++){
            tris[c]=ts->getp(j);
        }
    }

    bvh=PolygonBVHUtil::createBVHR(tris,0,trinum,0);
    printf("%f,%f,%f,%f,%f,%f,\n",bvh->bounds.xmin,bvh->bounds.xmax,bvh->bounds.ymin,bvh->bounds.ymax,bvh->bounds.zmin,bvh->bounds.zmax);
}

void BVHRayTracer::getIntersectionFromCamera(float x,float y,RayIntersection& intersection){
    RayObject ray;
    scene->getCamera()->getRayObject(x,y,ray);
    ray.id=NULL;
    ray.ior=1.0f;
    getIntersection(ray,intersection);
}

void BVHRayTracer::getIntersection(const RayObject& ray,RayIntersection& intersection){
    intersection.detect=0;
    intersection.depth=ray.maxDepth;
    intersection.ray=ray.direct;

    RayObject cpRay=ray;
    RayIntersection tempInt;
    PolygonBVHUtil::traverse(bvh,cpRay,intersection);
//    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
//    int n=ms->getSize();
//
//    for(int i=0;i<n;i++){
//        PolygonMesh* pm=ms->get(i);
//        LargeArray<Triangle>* ts=pm->getTriangleList();
//        int m=ts->length();
//        for(int j=0;j<m;j++){
//            Triangle* ct=ts->getp(j);
//            if(ct==cpRay.id){
//                continue;
//            }
//            int res=ct->getIntersection(cpRay,tempInt);
//            if(res){
//                if(intersection.depth>tempInt.depth){
//                    intersection=tempInt;
//                    cpRay.maxDepth=tempInt.depth;
//                }
//            }
//        }
//    }



    ArrayList<IsHair*>* hs=scene->getIsHairList();
    int n2=hs->getSize();
    for(int i=0;i<n2;i++){
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

