/*
 * ishair.h
 *
 *  Created on: 2013/08/09
 *      Author: take
 */

#ifndef ISHAIR_H_
#define ISHAIR_H_

#include "structDef.h"
#include "renderable.h"

struct IsHairNode;
struct IsHairBone;
class IsHairSkeleton;
struct IsHairBVHNode;
class IsHair;

struct IsHairNode{
    Vector4D loc;
    float w;
    float r;
};

struct IsHairBone{
    Vector4D cpoint[4];
    Vector4D sa,sb,sc,sd;
    float ra,rb,rc,rd,va,vb,vc,vd;
    AABB3D bounds;
};

class IsHairSkeleton{
    friend class IsHair;
    friend class IsHairLoader;

private:
    IsHairNode* node;
    IsHairBone* bone;
    int n;

public:
    IsHairSkeleton(int n);
    ~IsHairSkeleton();
    void fix();
};

struct IsHairBVHNode{
    IsHairBVHNode* c0;
    IsHairBVHNode* c1;
    IsHairBone* bone;
    AABB3D bounds;
    IsHairBVHNode(){
        c0=c1=0;
        bone=0;
    }
    ~IsHairBVHNode(){
        SAFE_DELETE_O(c0);
        SAFE_DELETE_O(c1);
    }
};

class IsHair:public Renderable{
    friend class IsHairUtil;
    friend class IsHairLoader;

private:
    IsHairSkeleton** skeletons;
    float threshold;
    int n;
    IsHairBVHNode* bvh;
    Material* material;
public:
    IsHair(int n);
    ~IsHair();
    void fix();
    int getIntersection(const RayObject& ray,RayIntersection& intersection);
    int getNormal(const RayIntersection& intersection,Vector4D& normal);
    void getPoint(const RayIntersection& intersection,Vector4D& p);
    void getUV(const RayIntersection& intersection,VectorUV& uv);
    void getTangent(const RayIntersection& intersection,Vector4D& tangent);
    Material* getMaterial();
    void setMaterial(Material* _material);
};

class IsHairLoader{
public:
    static IsHair* loadIsHairObject(const char* data,int offset);
    static IsHair* loadIsHairObject(const char* fileName);
    static IsHair* createTestIsHairObject();
};

class IsHairUtil{
public:
    static inline int checkAABB(const Vector4D& v,const AABB3D& aabb){
        if(v.x>aabb.xmax||v.x<aabb.xmin||v.y>aabb.ymax||v.y<aabb.ymin||v.z>aabb.zmax||v.z<aabb.zmin){
            return 0;
        }else{
            return 1;
        }
    }
    static int calcWaight(const IsHairBone& bone,const Vector4D& v,Vector4D& ret);
    static int calcFunction(const IsHairBVHNode* bvh,const Vector4D& v,Vector4D& ret);
    static float calcHairFunction(const IsHair* hair,const Vector4D& v);
    static void calcTangent(const IsHair* hair,const Vector4D& v,Vector4D& tangent);
    static int calcIntersection(const IsHair* hair,const Vector4D& v,float* fnc);
    static int rayMarching(const IsHair* hair,const RayObject& ray,RayIntersection& intersection);
    static void SplineToBezier(int n,IsHairNode* node,IsHairBone* bone);
    static IsHairBVHNode* createBVH(int n,IsHairBone** bones);
};

#endif /* ISHAIR_H_ */
