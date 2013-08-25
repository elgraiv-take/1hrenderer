/**
 * @file light.cpp
 *
 * @author take
 */


#include "photon.h"
#include "vectorMath.h"


LightSource::LightSource(Scene* scene){
    int lnum=0;
    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
    int n=ms->getSize();
    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct->getMaterial()->emission){
                lnum++;
            }
        }
    }
    num=lnum;
    sources=new Triangle*[lnum];
    lnum=0;
    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct->getMaterial()->emission){
                sources[lnum]=ct;
                lnum++;
            }
        }
    }
    areas=new float[num];
    float sum=0.0f;
    for(int i=0;i<num;i++){
        sum+=sources[i]->getArea();
        areas[i]=sum;
    }
    for(int i=0;i<num;i++){
        areas[i]/=sum;
    }
}
LightSource::~LightSource(){
    SAFE_DELETE_A(sources);
    SAFE_DELETE_A(areas);
}
void LightSource::getRandumPhoton(RayObject& ray,ColorRGBA& color){
    float r=Xorshift::nextf();
    int index=0;
    for(int i=0;i<num;i++){
        if(r<areas[i]){
            index=i;
            break;
        }
    }
    float k=Xorshift::nextf();
    float l=Xorshift::nextf();
    if(k+l>1){
        k=1.0f-k;
        l=1.0f-l;
    }

    sources[index]->getPoint(k,l,ray.start);
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.ior=1.0f;
    VectorMath::setRandomSphereVector(ray.direct);
    Vector4D no;
    sources[index]->getNormal(k,l,no);
    float dot=VectorMath::dot(ray.direct,no);
    dot=abs(dot);
    if(dot<0){
        ray.direct.x*=-1;
        ray.direct.y*=-1;
        ray.direct.z*=-1;
    }
    ray.id=sources[index];
    color.r=sources[index]->getMaterial()->emitPower*dot*sources[index]->getMaterial()->emitColor.r;
    color.g=sources[index]->getMaterial()->emitPower*dot*sources[index]->getMaterial()->emitColor.g;
    color.b=sources[index]->getMaterial()->emitPower*dot*sources[index]->getMaterial()->emitColor.b;

}
