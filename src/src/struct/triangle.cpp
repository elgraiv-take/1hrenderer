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
    intersection.type=POLYGON_MESH;

    return 1;
}

float Triangle::getArea(){
    return area;
}

void Triangle::getPoint(const RayIntersection& ri,Vector4D& p){
    p.x=v0->co.x+ri.k*e1.x+ri.l*e2.x;
    p.y=v0->co.y+ri.k*e1.y+ri.l*e2.y;
    p.z=v0->co.z+ri.k*e1.z+ri.l*e2.z;
}
int Triangle::getNormal(const RayIntersection& ri,Vector4D& normal){
    normal.x=((1.0f-ri.k-ri.l)*v0->normal.x+ri.k*v1->normal.x+ri.l*v2->normal.x);
    normal.y=((1.0f-ri.k-ri.l)*v0->normal.y+ri.k*v1->normal.y+ri.l*v2->normal.y);
    normal.z=((1.0f-ri.k-ri.l)*v0->normal.z+ri.k*v1->normal.z+ri.l*v2->normal.z);
    float check=VectorMath::dot(ri.ray,normal);
    if(check<0.0f){
        normal.x*=-1.0f;
        normal.y*=-1.0f;
        normal.z*=-1.0f;
        return -1;
    }
    return 1;

}

void Triangle::getUV(const RayIntersection& ri,VectorUV& uv){

    uv.u=((1.0f-ri.k-ri.l)*uv0.u+ri.k*uv1.u+ri.l*uv2.u);
    uv.v=((1.0f-ri.k-ri.l)*uv0.v+ri.k*uv1.v+ri.l*uv2.v);
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

}

void Triangle::getTangent(const RayIntersection& intersection,Vector4D& tangent){
    tangent.x=0.0f;
    tangent.y=0.0f;
    tangent.z=1.0f;

}

Material* Triangle::getMaterial(){
    return material;
}

void Triangle::setMaterial(Material* _material){
    material=_material;
}

void Triangle::setVertex(Vertex* _v0,Vertex* _v1,Vertex* _v2){
    v0=_v0;
    v1=_v1;
    v2=_v2;
}

void Triangle::setUV(VectorUV& _uv0,VectorUV& _uv1,VectorUV& _uv2){
    uv0=_uv0;
    uv1=_uv1;
    uv2=_uv2;
}
void Triangle::fix(){
    VectorMath::sub(v1->co,v0->co,e1);
    VectorMath::sub(v2->co,v0->co,e2);
    VectorMath::initAABB(aabb);
    VectorMath::putVtoAABB(v0->co,aabb);
    VectorMath::putVtoAABB(v1->co,aabb);
    VectorMath::putVtoAABB(v2->co,aabb);
    Vector4D c;
    VectorMath::cross(e1,e2,c);
    area=VectorMath::getNorm(c)/2.0f;

}
