/*
 * rayhpcontainer.cpp
 *
 *  Created on: 2013/07/02
 *      Author: take
 */


#include "renderer.h"
#include "raytracer.h"
#include "vectorMath.h"
#include "brdf.h"
#include "paramdef.h"
#include <omp.h>
#include "photon.h"

RayHitPointContainer::RayHitPointContainer(RayHitPoint* _hitpoints,int n){
    hitpoints=_hitpoints;
    num=n;
    sort[0]=new RayHitPoint*[n];
    sort[1]=new RayHitPoint*[n];
    sort[2]=new RayHitPoint*[n];
    kdTree=new RayHitPoint*[n];
    childrenCounter=new int[n];
    maxRadius=new float[n];
    divAxis=new int[n];
    for(int i=0;i<n;i++){

        maxRadius[i]=PPM_DEFAULT_RADIUS;
        hitpoints[i].kdtIndex=0;
        sort[0][i]=&(hitpoints[i]);
        sort[1][i]=&(hitpoints[i]);
        sort[2][i]=&(hitpoints[i]);
        childrenCounter[i]=1;
        kdTree[i]=0;
    }
    for(int i=n-1;i>0;i--){
        int pi=(i-1)/2;
        childrenCounter[pi]+=childrenCounter[i];
    }
    printf("sort\n");
    sortPoint(sort[AXIS_X],AXIS_X);
    sortPoint(sort[AXIS_Y],AXIS_Y);
    sortPoint(sort[AXIS_Z],AXIS_Z);
    printf("sort comp\n");
    constractKdt(0,AXIS_X);
    for(int i=0;i<n;i++){
        kdTree[hitpoints[i].kdtIndex]=(&(hitpoints[i]));
    }
}

RayHitPointContainer::~RayHitPointContainer(){
    SAFE_DELETE_A(sort[0]);
    SAFE_DELETE_A(sort[1]);
    SAFE_DELETE_A(sort[2]);
    SAFE_DELETE_A(kdTree);
    SAFE_DELETE_A(childrenCounter);
    SAFE_DELETE_A(divAxis);
    SAFE_DELETE_A(maxRadius);
}

void RayHitPointContainer::constractKdt(int index,int axis){
    int* counter=new int[num];
    for(int i=0;i<num;i++){
        counter[i]=0;
    }
    index=0;
    axis=AXIS_X;
    while(index<num){
        int last=index*2+1;
        if(last>num){
            last=num;
        }

        for(int i=0;i<num;i++){
            RayHitPoint* hp=sort[axis][i];
            int in=hp->kdtIndex;
            if(in<index){
                continue;
            }
            int li=in*2+1;
            if(li>=num){
                continue;
            }
            if(counter[in]<childrenCounter[li]){
                hp->kdtIndex=li;
            }else if(counter[in]>childrenCounter[li]){
                hp->kdtIndex=li+1;
            }
            counter[in]++;
        }

        for(int i=index;i<last;i++){
            divAxis[i]=axis;
        }
        axis=NEXT_AXIS(axis);
        index=last;
    }
    SAFE_DELETE_A(counter);
}

void RayHitPointContainer::heepup(RayHitPoint** array,int axis,int index){
    if(index==0){
        return;
    }
    int pindex=(index-1)/2;
    if(array[index]->ri.point.xyzw[axis]>array[pindex]->ri.point.xyzw[axis]){
        RayHitPoint* tmp=array[index];
        array[index]=array[pindex];
        array[pindex]=tmp;
        heepup(array,axis,pindex);
    }
}
void RayHitPointContainer::heepdown(RayHitPoint** array,int axis,int index,int size){
    int lc=index*2+1;

    if(!(lc<size)){
        return;
    }

    float max=array[index]->ri.point.xyzw[axis];
    int nindex=index;
    if(max<array[lc]->ri.point.xyzw[axis]){
        max=array[lc]->ri.point.xyzw[axis];
        nindex=lc;
    }
    int rc=index*2+2;
    if((rc<size)&&(max<array[rc]->ri.point.xyzw[axis])){
        nindex=rc;
    }

    if(nindex!=index){
        RayHitPoint* tmp=array[index];
        array[index]=array[nindex];
        array[nindex]=tmp;
        heepdown(array,axis,nindex,size);
    }
}

void RayHitPointContainer::sortPoint(RayHitPoint** array,int axis){
    for(int i=1;i<num;i++){
//        printf("%d\n",i);
        heepup(array,axis,i);
    }
    for(int i=num-1;i>0;i--){
        RayHitPoint* tmp=array[i];
        array[i]=array[0];
        array[0]=tmp;
        heepdown(array,axis,0,i);
    }
}
void RayHitPointContainer::applyPhotonSub(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius,int index){

    int axis=divAxis[index];
    float axisDist=photon.point.xyzw[axis]-kdTree[index]->ri.point.xyzw[axis];
    int lc=index*2+1;
    int rc=index*2+2;
    float d=VectorMath::distance(photon.point,kdTree[index]->ri.point);
//    printf("----------------------%f %f %f\n",photon.point.x,photon.point.y,photon.point.z);
//    printf("-----------------------------%f\n",d);
    if(d<kdTree[index]->radius){
        int of=(*offset);
        if(of<max){
            rpm[of].ray=kdTree[index];
//            printf("%x-%x \n",rpm[(*offset)].ray,kdTree[index]);
            rpm[of].photon=photon;
            rpm[of].r=d;
//            printf("-----------------------------%f\n",d);
            (*offset)++;
        }
    }
    if(axisDist<0){
        if(lc<num){
            applyPhotonSub(rpm,photon,offset,max,radius,lc);
        }
        if(rc<num){
            if(axisDist>-maxRadius[rc]){
                applyPhotonSub(rpm,photon,offset,max,radius,rc);
            }
        }
    }else{
        if(rc<num){
            applyPhotonSub(rpm,photon,offset,max,radius,rc);
        }
        if(lc<num){
            if(axisDist<maxRadius[lc]){

                applyPhotonSub(rpm,photon,offset,max,radius,lc);
            }
        }
    }
}
void RayHitPointContainer::applyPhoton(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius){
//    printf("%f,%f,%f\n",photon.point.x,photon.point.y,photon.point.z);
    applyPhotonSub(rpm,photon,offset,max,radius,0);
}

void RayHitPointContainer::updateMaxRadiusAll(){
    for(int i=num-1;i>=0;i--){
        float max=kdTree[i]->radius;
        int lc=i*2+1;
        int rc=i*2+2;
        if(lc<num){
            max=maxRadius[lc]>max?maxRadius[lc]:max;
            if(rc<num){
                max=maxRadius[rc]>max?maxRadius[rc]:max;
            }
        }
        maxRadius[i]=max;
    }
    printf("maxR-%f\n",maxRadius[0]);
}

void RayHitPointContainer::updateMaxRadius(int index){
    while(true){
        float max=kdTree[index]->radius;
        int lc=index*2+1;
        int rc=index*2+2;
        if(lc<num){
            max=maxRadius[lc]>max?maxRadius[lc]:max;
            if(rc<num){
                max=maxRadius[rc]>max?maxRadius[rc]:max;
            }
        }
        if(maxRadius[index]>max){
            maxRadius[index]=max;
            if(index!=0){
                index=(index-1)/2;
            }else{
                break;
            }
        }else{
            break;
        }
    }
}

