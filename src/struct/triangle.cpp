/**
 * @file triangle.cpp
 *
 * @author take
 */

#include "dataStructure.h"
#include "vectorMath.h"
#include "brdf.h"

Triangle::Triangle(){
    material=NULL;
}

int Triangle::getIntersection(const RayObject& ray,RayIntersection& intersection){
    float deta=VectorMath::det(
            e1.x,e2.x,-ray.direct.x,
            e1.y,e2.y,-ray.direct.y,
            e1.z,e2.z,-ray.direct.z
            );
    if(abs(deta)<FLOAT_MIN_VAL){
        return 0;
    }

    float ex=ray.start.x-v0->co.x;
    float ey=ray.start.y-v0->co.y;
    float ez=ray.start.z-v0->co.z;

    float k=VectorMath::det(
            ex,e2.x,-ray.direct.x,
            ey,e2.y,-ray.direct.y,
            ez,e2.z,-ray.direct.z
            )/deta;
    float l=VectorMath::det(
            e1.x,ex,-ray.direct.x,
            e1.y,ey,-ray.direct.y,
            e1.z,ez,-ray.direct.z
            )/deta;
    float d=VectorMath::det(
            e1.x,e2.x,ex,
            e1.y,e2.y,ey,
            e1.z,e2.z,ez
            )/deta;
//    printf("%f \n",deta);
//    printf("%f %f %f \n",e1.x,e1.y,e1.z);
//    printf("%f %f %f \n",e2.x,e2.y,e2.z);
//    printf("%f %f %f \n",v0->co.x,v0->co.y,v0->co.z);
//    printf("%f %f %f \n",v1->co.x,v1->co.y,v1->co.z);
//    printf("%f %f %f \n",v2->co.x,v2->co.y,v2->co.z);
//    printf("%f %f %f\n\n",k,l,d);
    if(k<0||l<0||(k+l)>1||d<ray.minDepth||d>ray.maxDepth){
        return 0;
    }
    intersection.depth=d;
    intersection.detect=1;
    intersection.k=k;
    intersection.l=l;
    intersection.id=this;
    intersection.ray.x=-ray.direct.x;
    intersection.ray.y=-ray.direct.y;
    intersection.ray.z=-ray.direct.z;
    intersection.ior=ray.ior;
//    intersection.point.x=v0->co.x+k*e1.x+l*e2.x;
//    intersection.point.y=v0->co.y+k*e1.y+l*e2.y;
//    intersection.point.z=v0->co.z+k*e1.z+l*e2.z;
//    intersection.point.x=((1.0f-k-l)*v0->normal.x+k*v1->normal.x+l*v2->normal.x)/2.0f+0.5;
//    intersection.point.y=((1.0f-k-l)*v0->normal.y+k*v1->normal.y+l*v2->normal.y)/2.0f+0.5;
//    intersection.point.z=((1.0f-k-l)*v0->normal.z+k*v1->normal.z+l*v2->normal.z)/2.0f+0.5;
//    intersection.point.x=((1.0f-k-l)*v0->normal.x+k*v1->normal.x+l*v2->normal.x)/2.0f+0.5;
//    intersection.point.y=((1.0f-k-l)*v0->normal.y+k*v1->normal.y+l*v2->normal.y)/2.0f+0.5;
//    intersection.point.z=((1.0f-k-l)*v0->normal.z+k*v1->normal.z+l*v2->normal.z)/2.0f+0.5;
    return 1;
}

void Triangle::getPoint(float k,float l,Vector4D& p){
    p.x=v0->co.x+k*e1.x+l*e2.x;
    p.y=v0->co.y+k*e1.y+l*e2.y;
    p.z=v0->co.z+k*e1.z+l*e2.z;
}
void Triangle::getNormal(float k,float l,Vector4D& normal){
    normal.x=((1.0f-k-l)*v0->normal.x+k*v1->normal.x+l*v2->normal.x);
    normal.y=((1.0f-k-l)*v0->normal.y+k*v1->normal.y+l*v2->normal.y);
    normal.z=((1.0f-k-l)*v0->normal.z+k*v1->normal.z+l*v2->normal.z);
//    VectorMath::cross(e1,e2,normal);
//    VectorMath::normalize(normal);

}
Material* Triangle::getMaterial(){
    return material;
}

void Triangle::setMaterial(Material* _material){
    material=_material;
}

void Triangle::brdf(float k,float l,Vector4D& in,Vector4D& out,ColorRGBA& ret){
    Vector4D normal;
    Vector4D tan;
    getNormal(k,l,normal);
    material->brdf->function(normal,in,out,tan,ret);
}

void Triangle::setVertex(Vertex* _v0,Vertex* _v1,Vertex* _v2){
    v0=_v0;
    v1=_v1;
    v2=_v2;
}

void Triangle::fix(){
    VectorMath::sub(v1->co,v0->co,e1);
    VectorMath::sub(v2->co,v0->co,e2);
    VectorMath::initAABB(aabb);
    VectorMath::putVtoAABB(v0->co,aabb);
    VectorMath::putVtoAABB(v1->co,aabb);
    VectorMath::putVtoAABB(v2->co,aabb);
//    printf("%f %f %f\n",e1.x,e1.y,e1.z);
//    printf("%f %f %f\n",e2.x,e2.y,e2.z);
//    printf("%f %f %f\n",v0->co.x,v0->co.y,v0->co.z);
//    printf("%f %f %f\n",v1->co.x,v1->co.y,v1->co.z);
//    printf("%f %f %f\n",v2->co.x,v2->co.y,v2->co.z);
//    printf("\n");
}
