/*
 * ishair.cpp
 *
 *  Created on: 2013/08/12
 *      Author: take
 */

#include "ishair.h"
#include "vectorMath.h"
#include "rayUtil.h"

IsHairSkeleton::IsHairSkeleton(int _n){
    n=_n;
    node=new IsHairNode[_n+1];
    bone=new IsHairBone[_n];
}
IsHairSkeleton::~IsHairSkeleton(){
    SAFE_DELETE_A(node);
    SAFE_DELETE_A(bone);
}
void IsHairSkeleton::fix(){
    IsHairUtil::SplineToBezier(n,node,bone);
    for(int i=0;i<n;i++){
        float rmax=MAX(node[i].r,node[i+1].r);
//        float rmax=0.0f;
        bone[i].bounds.xmax=MAX(MAX(bone[i].cpoint[0].x,bone[i].cpoint[1].x),MAX(bone[i].cpoint[2].x,bone[i].cpoint[3].x))+rmax;
        bone[i].bounds.ymax=MAX(MAX(bone[i].cpoint[0].y,bone[i].cpoint[1].y),MAX(bone[i].cpoint[2].y,bone[i].cpoint[3].y))+rmax;
        bone[i].bounds.zmax=MAX(MAX(bone[i].cpoint[0].z,bone[i].cpoint[1].z),MAX(bone[i].cpoint[2].z,bone[i].cpoint[3].z))+rmax;
        bone[i].bounds.xmin=MIN(MIN(bone[i].cpoint[0].x,bone[i].cpoint[1].x),MIN(bone[i].cpoint[2].x,bone[i].cpoint[3].x))-rmax;
        bone[i].bounds.ymin=MIN(MIN(bone[i].cpoint[0].y,bone[i].cpoint[1].y),MIN(bone[i].cpoint[2].y,bone[i].cpoint[3].y))-rmax;
        bone[i].bounds.zmin=MIN(MIN(bone[i].cpoint[0].z,bone[i].cpoint[1].z),MIN(bone[i].cpoint[2].z,bone[i].cpoint[3].z))-rmax;
        bone[i].bounds.center.x=(bone[i].bounds.xmax+bone[i].bounds.xmin)*0.5f;
        bone[i].bounds.center.y=(bone[i].bounds.ymax+bone[i].bounds.ymin)*0.5f;
        bone[i].bounds.center.z=(bone[i].bounds.zmax+bone[i].bounds.zmin)*0.5f;
//        bone[i].r0=node[i].r;
//        bone[i].r1=node[i+1].r;
//        bone[i].v0=node[i].w;
//        bone[i].v1=node[i+1].w;
    }
}

//-----------------------------------------------------------

IsHair::IsHair(int _n){
    n=_n;
    skeletons=new IsHairSkeleton*[_n];
    bvh=NULL;
    threshold=0.0f;
}
IsHair::~IsHair(){
    SAFE_DELETE_O(bvh);
    for(int i=0;i<n;i++){
        SAFE_DELETE_O(skeletons[i]);
    }
    SAFE_DELETE_A(skeletons)
}
void IsHair::fix(){
    int bnum=0;
    for(int i=0;i<n;i++){
        skeletons[i]->fix();
        bnum+=skeletons[i]->n;
    }
    IsHairBone** bones=new IsHairBone*[bnum];

    for(int i=0,c=0;i<n;i++){
        for(int j=0;j<skeletons[i]->n;j++,c++){
            bones[c]=&(skeletons[i]->bone[j]);
        }
    }
    bvh=IsHairUtil::createBVH(bnum,bones);
    printf("%f,%f,%f,%f,%f,%f,\n",bvh->bounds.xmin,bvh->bounds.xmax,bvh->bounds.ymin,bvh->bounds.ymax,bvh->bounds.zmin,bvh->bounds.zmax);

    SAFE_DELETE_A(bones);
}
int IsHair::getIntersection(const RayObject& ray,RayIntersection& intersection){
    if(IsHairUtil::rayMarching(this,ray,intersection)){
        intersection.id=this;
        return 1;
    }
    return 0;
}

void IsHair::getUV(const RayIntersection& intersection,VectorUV& uv){
    uv.u=0.0f;
    uv.v=0.0f;
}

int IsHair::getNormal(const RayIntersection& intersection,Vector4D& normal){

    float res=IsHairUtil::calcHairFunction(this,intersection.point);
    float delx=0.00001;
    float dely=0.00001f;
    float delz=0.00001f;
    float resx=0.0f;
    float resy=0.0f;
    float resz=0.0f;
    Vector4D nv;
    while(true){
        nv.x=intersection.point.x+delx;
        nv.y=intersection.point.y;
        nv.z=intersection.point.z;
        resx=-(IsHairUtil::calcHairFunction(this,nv)-res)/delx;
        nv.x=intersection.point.x;
        nv.y=intersection.point.y+dely;
        nv.z=intersection.point.z;

        resy=-(IsHairUtil::calcHairFunction(this,nv)-res)/dely;
        nv.x=intersection.point.x;
        nv.y=intersection.point.y;
        nv.z=intersection.point.z+delz;
        resz=-(IsHairUtil::calcHairFunction(this,nv)-res)/delz;

        if(fabs(resx)<0.0000001f&&fabs(resy)<0.0000001f&&fabs(resz)<0.0000001f){
            delx+=delx;
            dely+=dely;
            delz+=delz;
        }else{
            break;
        }

    }
    float lenN=sqrt(resx*resx+resy*resy+resz*resz);
    normal.x=resx/lenN;
    normal.y=resy/lenN;
    normal.z=resz/lenN;
    return 1;
}

void IsHair::getTangent(const RayIntersection& intersection,Vector4D& tangent){
    IsHairUtil::calcTangent(this,intersection.point,tangent);
}

void IsHair::getPoint(const RayIntersection& intersection,Vector4D& p){
    p=intersection.point;
}
Material* IsHair::getMaterial(){
    return material;
}
void IsHair::setMaterial(Material* _material){
    material=_material;
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------
//---------------------------------------------------------------------------------

namespace IsHairRaymarchingUtil {

int getRaymarchiRange(const IsHairBVHNode* bvh,const RayObject& ray,float* min,float* max){
    float mx,mn;
    int hit=RaytraceUtil::checkRayRange(bvh->bounds,ray,&mn,&mx);

    if(!hit){
        return 0;
    }
//    printf("%f,%f----%f,%f\n",bvh->bounds.ymin,bvh->bounds.ymax,mn,mx);
//    printf("****\n");
    if(bvh->bone){
        (*min)=mn;
        (*max)=mx;
    }else{
        float mn1,mx1,mn2,mx2;
        mx=0.0f;
        mn=FLOAT_MAX_VAL;
        if(getRaymarchiRange(bvh->c0,ray,&mn1,&mx1)){
            mx=MAX(mx,mx1);
            mn=MIN(mn,mn1);
        }
        if(getRaymarchiRange(bvh->c1,ray,&mn2,&mx2)){
            mx=MAX(mx,mx2);
            mn=MIN(mn,mn2);
        }
        (*min)=mn;
        (*max)=mx;
    }
    return 1;
}

}  // namespace IsHairRaymarchingUtil

namespace SkeletonFunction{

//#define skeletonFunc skeletonFunc2ex
#define skeletonFunc skeletonFunc2

inline float skeletonFunc2(float r){
    if(r>1){
        return 0.0f;
    }
    if(r>(1.0f/3.0f)){
        float imx=1.0f-r;
        return 1.5f*imx*imx;
    }
    return 1.0f-3.0f*r*r;
}

inline float skeletonFunc0(float r){
    if(r>1){
        return 0.0f;
    }

    if(r<0.5){
        return 1.0f;
    }

    return 0.0f;
}

inline float skeletonFunc1(float r){
    if(r>1){
        return 0.0f;
    }

    return 1.0f-r;
}
inline float skeletonFunc2ex(float r){
    if(r>1){
        return 0.0f;
    }
    float f1=skeletonFunc0(r);
    float f2=skeletonFunc2(r);
    float rate=pow(r,0.2f);
    return f1*(1.0f-rate)+f2*rate;
}


inline float square(float u,float v,float r){
    float d1=u-r;
    float d2=-r-u;
    float d3=v-r;
    float d4=-r-v;
    float dist=MAX(MAX(d1,d2),MAX(d3,d4));
    return dist;
}

inline void CubicSpline3D(const IsHairBone& bone,float s,Vector4D& ret){
    float s2=s*s;
    float s3=s2*s;
    ret.x=bone.sa.x*s3+bone.sb.x*s2+bone.sc.x*s+bone.sd.x;
    ret.y=bone.sa.y*s3+bone.sb.y*s2+bone.sc.y*s+bone.sd.y;
    ret.z=bone.sa.z*s3+bone.sb.z*s2+bone.sc.z*s+bone.sd.z;
}

inline void CubicSpline3DD(const IsHairBone& bone,float s,Vector4D& ret){
    float s2=s*s;
    ret.x=3.0f*bone.sa.x*s2+2.0f*bone.sb.x*s+bone.sc.x;
    ret.y=3.0f*bone.sa.y*s2+2.0f*bone.sb.y*s+bone.sc.y;
    ret.z=3.0f*bone.sa.z*s2+2.0f*bone.sb.z*s+bone.sc.z;
}

inline void CubicSpline3DDD(const IsHairBone& bone,float s,Vector4D& ret){
    ret.x=6.0f*bone.sa.x*s+2.0f*bone.sb.x;
    ret.y=6.0f*bone.sa.y*s+2.0f*bone.sb.y;
    ret.z=6.0f*bone.sa.z*s+2.0f*bone.sb.z;
}

float funcDsD(const IsHairBone& bone,float s,const Vector4D& v){
    Vector4D fds;
    Vector4D fs;
    SkeletonFunction::CubicSpline3D(bone,s,fs);
    SkeletonFunction::CubicSpline3DD(bone,s,fds);
    return 2.0f*(fds.x*(fs.x-v.x)+fds.y*(fs.y-v.y)+fds.z*(fs.z-v.z));
}
float funcDsDD(const IsHairBone& node,float s,const Vector4D& v){
    Vector4D fs;
    Vector4D fds;
    Vector4D fdds;
    SkeletonFunction::CubicSpline3D(node,s,fs);
    SkeletonFunction::CubicSpline3DD(node,s,fds);
    SkeletonFunction::CubicSpline3DDD(node,s,fdds);
    return 2.0f*(fdds.x*(fs.x-v.x)+fds.x*fds.x+fdds.y*(fs.y-v.y)+fds.y*fds.y+fdds.z*(fs.z-v.z)+fds.z*fds.z);
}
}

void IsHairUtil::calcTangent(const IsHair* hair,const Vector4D& v,Vector4D& tangent){
    Vector4D acm;
    int check=calcFunction(hair->bvh,v,acm);
    if(!check){
        printf("--------------");
        acm.x=1.0f;
    }
    VectorMath::normalize(acm);
    tangent=acm;
}

int IsHairUtil::calcWaight(const IsHairBone& bone,const Vector4D& v,Vector4D& ret){
    Vector4D t1,t2,v1,v2;
    SkeletonFunction::CubicSpline3DD(bone,0.0f,t1);
    SkeletonFunction::CubicSpline3DD(bone,1.0f,t2);
    VectorMath::sub(v,bone.cpoint[0],v1);
    VectorMath::sub(v,bone.cpoint[3],v2);
    if(VectorMath::dot(t1,v1)<0.0f||VectorMath::dot(t2,v2)>0.0f){
        return 0;
    }

    float ds1=VectorMath::distance(bone.cpoint[0],v);
    float ds3=VectorMath::distance(bone.cpoint[3],v);
    float s2=0.5f;
    Vector4D tvec;
    for(int j=0;j<4;j++){
        SkeletonFunction::CubicSpline3D(bone,s2,tvec);
        float ds2=VectorMath::distance(tvec,v);
        float s22=s2*s2;
        float sn=((s22-1.0f)*+ds1+ds2+(-s22)*ds3)/(((s2-1.0f)*+ds1+ds2+(-s2)*ds3)*2.0f);
        s2=sn;
    }
    for(int j=0;j<4;j++){
        s2=s2-(SkeletonFunction::funcDsD(bone,s2,v)/SkeletonFunction::funcDsDD(bone,s2,v));
    }
    if(s2<0.0f){
        s2=0.0f;
    }
    if(s2>1.0f){
        s2=1.0f;
    }
    SkeletonFunction::CubicSpline3D(bone,s2,tvec);
    float r=VectorMath::distance(tvec,v);

    float n12=s2;
    float n22=1.0f-s2;

//    float ir=(n22*bone.r0+n12*bone.r1);
//    float iv=(n22*bone.v0+n12*bone.v1);
    float ir=bone.ra*s2*s2*s2+bone.rb*s2*s2+bone.rc*s2+bone.rd;
    float iv=bone.va*s2*s2*s2+bone.vb*s2*s2+bone.vc*s2+bone.vd;
    float fi=SkeletonFunction::skeletonFunc(r/ir);

    float amount=fi*iv;
    Vector4D tan;
    SkeletonFunction::CubicSpline3DD(bone,s2,tan);
    VectorMath::normalize(tan);

    ret.x+=tan.x*amount;
    ret.y+=tan.y*amount;
    ret.z+=tan.z*amount;

    return 1;
}
int IsHairUtil::calcFunction(const IsHairBVHNode* bvh,const Vector4D& v,Vector4D& ret){
    if(!checkAABB(v,bvh->bounds)){
        return 0;
    }
    //printf("-");
    if(bvh->bone){
        return calcWaight(*(bvh->bone),v,ret);
    }else{
        return calcFunction(bvh->c0,v,ret)+calcFunction(bvh->c1,v,ret);
    }

}

float IsHairUtil::calcHairFunction(const IsHair* hair,const Vector4D& v){
    Vector4D acm;
    int check=calcFunction(hair->bvh,v,acm);
    float sumW=VectorMath::getNorm(acm);
    float f=sumW-hair->threshold;
    return f;
}
int IsHairUtil::calcIntersection(const IsHair* hair,const Vector4D& v,float* fnc){
    Vector4D acm;
    int check=calcFunction(hair->bvh,v,acm);
//    printf("%d",check);
    if(!check){
        return 0;
    }
    float sumW=VectorMath::getNorm(acm);
//    printf("%f\n",sumW);
    float f=sumW-hair->threshold;
    (*fnc)=f;
    if(f<0.0){
        return 0;
    }else{

        return 1;
    }
}
int IsHairUtil::rayMarching(const IsHair* hair,const RayObject& ray,RayIntersection& intersection){
    float maxDepth,minDepth;
    int ck=IsHairRaymarchingUtil::getRaymarchiRange(hair->bvh,ray,&minDepth,&maxDepth);
    if(!ck){
        return 0;
    }
//    printf("----\n");
    if(minDepth<0.02){
        minDepth=0.02;
    }
    float maxstep=0.01;
    float minstep=0.0005;
    float dstep=0.005;
    float depth=minDepth;
    Vector4D v=ray.start;
    v.x+=ray.direct.x*minDepth;
    v.y+=ray.direct.y*minDepth;
    v.z+=ray.direct.z*minDepth;
    Vector4D stepR;
//    stepR.x=ray.direct.x*step;
//    stepR.y=ray.direct.y*step;
//    stepR.z=ray.direct.z*step;
//    printf("^^^^^^^%f,%f\n",minDepth,maxDepth);
//    printf("^^^^^^^%f,%f,%f^^^%f,%f,%f\n",v.x,v.y,v.z,minDepth,maxDepth,VectorMath::getNorm(stepR));//TODO a
    int cnt=0;
    float fnc=FLOAT_MAX_VAL;
    float pre=FLOAT_MAX_VAL;
    while(depth<=maxDepth){
        if(calcIntersection(hair,v,&fnc)){
//            printf("---%d\n",cnt);
            intersection.depth=depth;
            intersection.detect=1;
            intersection.point=v;
            intersection.ray.x=-ray.direct.x;
            intersection.ray.y=-ray.direct.y;
            intersection.ray.z=-ray.direct.z;
            intersection.ior=ray.ior;
            intersection.type=IS_HAIR;
            return 1;
        }
        if(fnc!=FLOAT_MAX_VAL&&pre!=FLOAT_MAX_VAL){
            float df=fnc-pre;
            if(df<=0.0f){
                dstep=maxstep;
            }else{
                dstep=((-fnc*dstep)/df)/2.0f;
//                printf("%f__%f__%f\n",pre,fnc,dstep);
                dstep=MAX(minstep,dstep);
                dstep=MIN(maxstep,dstep);
            }
        }

//        printf("f-%f\n",fnc);
//        v.x+=stepR.x;
//        v.y+=stepR.y;
//        v.z+=stepR.z;
        v.x+=ray.direct.x*dstep;
        v.y+=ray.direct.y*dstep;
        v.z+=ray.direct.z*dstep;

        depth+=dstep;
        cnt++;
        pre=fnc;
    }
//    printf("-------\n");
//    printf("_______%f,%f,%f__%d\n",v.x,v.y,v.z,cnt);//TODO b
    return 0;
}

namespace IsHairS2BUtil {
struct SplineParams{
    float a,b,c,d,t1,t2;
    SplineParams(){
        a=b=c=d=t1=t2=0.0f;
    }
};

inline void calcSB(int n,SplineParams* prms){
    prms[0].t1=prms[n].t1=0.0f;
    for(int i=1;i<n;i++){
        prms[i].t1=3.0f*(prms[i-1].d-2.0f*prms[i].d+prms[i+1].d);
    }
    prms[0].t2=0.0f;
    for(int i=1;i<n;i++){
        float t=4.0f-prms[i-1].t2;
        prms[i].t1=(prms[i].t1-prms[i-1].t1)/t;
        prms[i].t2=1.0f/t;
    }
    for(int i=n-1;i>0;i--){
        prms[i].t1=prms[i].t1-prms[i+1].t1*prms[i].t2;
    }
    for(int i=0;i<n+1;i++){
        prms[i].b=prms[i].t1;
    }
}

void SplineToBezierNode(SplineParams& sp,IsHairBone& bone,int axis){
    bone.cpoint[0].xyzw[axis]=sp.d;
    bone.cpoint[1].xyzw[axis]=sp.c/3.0f+bone.cpoint[0].xyzw[axis];
    bone.cpoint[2].xyzw[axis]=sp.b/3.0f+bone.cpoint[1].xyzw[axis]*2.0f-bone.cpoint[0].xyzw[axis];
}

void splineParams(int n,IsHairNode* node,IsHairBone* bone){
    SplineParams* vs=new SplineParams[n+1];
    SplineParams* rs=new SplineParams[n+1];

    for(int i=0;i<n+1;i++){
        vs[i].d=node[i].w;
        rs[i].d=node[i].r;
    }
    calcSB(n,vs);
    calcSB(n,rs);
    for(int i=0;i<n;i++){
        vs[i].c=vs[i+1].d-vs[i].d-((vs[i+1].b+2.0f*vs[i].b)/3.0f);
        rs[i].c=rs[i+1].d-rs[i].d-((rs[i+1].b+2.0f*rs[i].b)/3.0f);
    }
    for(int i=0;i<n;i++){
        vs[i].a=(vs[i+1].b-vs[i].b)/3.0f;
        rs[i].a=(rs[i+1].b-rs[i].b)/3.0f;
    }
    for(int i=0;i<n;i++){
        bone[i].va=vs[i].a;
        bone[i].vb=vs[i].b;
        bone[i].vc=vs[i].c;
        bone[i].vd=vs[i].d;
        bone[i].ra=rs[i].a;
        bone[i].rb=rs[i].b;
        bone[i].rc=rs[i].c;
        bone[i].rd=rs[i].d;
    }

    SAFE_DELETE_A(vs);
    SAFE_DELETE_A(rs);
}

void SplineToBezier1Axis(int n,IsHairNode* node,IsHairBone* bone,int axis){
    SplineParams* prms=new SplineParams[n+1];
    for(int i=0;i<n+1;i++){
        prms[i].d=node[i].loc.xyzw[axis];
    }
    calcSB(n,prms);
    for(int i=0;i<n;i++){
        prms[i].c=prms[i+1].d-prms[i].d-((prms[i+1].b+2.0f*prms[i].b)/3.0f);
    }
    for(int i=0;i<n;i++){
        prms[i].a=(prms[i+1].b-prms[i].b)/3.0f;
    }
    for(int i=0;i<n;i++){
        SplineToBezierNode(prms[i],bone[i],axis);
        bone[i].cpoint[3].xyzw[axis]=node[i+1].loc.xyzw[axis];
        bone[i].sa.xyzw[axis]=prms[i].a;
        bone[i].sb.xyzw[axis]=prms[i].b;
        bone[i].sc.xyzw[axis]=prms[i].c;
        bone[i].sd.xyzw[axis]=prms[i].d;
    }
    SAFE_DELETE_A(prms);
}

} // namespace IsHairS2BUtil

void IsHairUtil::SplineToBezier(int n,IsHairNode* node,IsHairBone* bone){
    IsHairS2BUtil::SplineToBezier1Axis(n,node,bone,0);
    IsHairS2BUtil::SplineToBezier1Axis(n,node,bone,1);
    IsHairS2BUtil::SplineToBezier1Axis(n,node,bone,2);
    IsHairS2BUtil::splineParams(n,node,bone);
}

namespace IsHairBVHUtil {

void heapDown(IsHairBone** bone,int index,int size,int axis){
    int lc=index*2+1;

    if(!(lc<size)){
        return;
    }

    float max=bone[index]->bounds.center.xyzw[axis];
    int nindex=index;
    if(max<bone[lc]->bounds.center.xyzw[axis]){
        max=bone[lc]->bounds.center.xyzw[axis];
        nindex=lc;
    }
    int rc=index*2+2;
    if((rc<size)&&(max<bone[rc]->bounds.center.xyzw[axis])){
        nindex=rc;
    }

    if(nindex!=index){
        IsHairBone* tmp=bone[index];
        bone[index]=bone[nindex];
        bone[nindex]=tmp;
        heapDown(bone,nindex,size,axis);
    }
}
void heapUp(IsHairBone** bone,int index,int axis){
    if(index==0){
        return;
    }
    int pindex=(index-1)/2;
    if(bone[index]->bounds.center.xyzw[axis]>bone[pindex]->bounds.center.xyzw[axis]){
        IsHairBone* tmp=bone[index];
        bone[index]=bone[pindex];
        bone[pindex]=tmp;
        heapUp(bone,pindex,axis);
    }
}

void sortBone(IsHairBone** bone,int size,int axis){
    for(int i=1;i<size;i++){
        heapUp(bone,i,axis);
    }
    for(int i=size-1;i>0;i--){
        IsHairBone* tmp=bone[i];
        bone[i]=bone[0];
        bone[0]=tmp;
        heapDown(bone,0,i,axis);
    }
}

IsHairBVHNode* createBVHR(IsHairBone** bones,int offset,int size,int axis){
    IsHairBVHNode* ret=new IsHairBVHNode();
    if(size==1){
        ret->bone=bones[offset];
        ret->bounds=bones[offset]->bounds;
        return ret;
    }
    sortBone(bones+offset,size,axis);
    int naxis=(axis+1)%3;
    int lsize=size/2;
    int rsize=size-lsize;
    ret->c0=createBVHR(bones,offset,lsize,naxis);
    ret->c1=createBVHR(bones,offset+lsize,rsize,naxis);
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

} // namespace IsHairBVHUtil

IsHairBVHNode* IsHairUtil::createBVH(int n,IsHairBone** bones){
    return IsHairBVHUtil::createBVHR(bones,0,n,0);
}

//-------------------------------------------------------------------------

IsHair* IsHairLoader::loadIsHairObject(const char* data,int offset){
    return NULL;
}
IsHair* IsHairLoader::loadIsHairObject(const char* fileName){
    FILE* fp=fopen(fileName,"r");
    if(!fp){
        return NULL;
    }
    int sn;
    fscanf(fp,"%d",&sn);
    IsHair* ret=new IsHair(sn);
    float thr;
    fscanf(fp,"%f",&thr);
    ret->threshold=thr;
    for(int i=0;i<sn;i++){
        int bn;
        fscanf(fp,"%d",&bn);
        ret->skeletons[i]=new IsHairSkeleton(bn);
        for(int j=0;j<bn+1;j++){
            float x,y,z,r,w;
            fscanf(fp,"%f,%f,%f,%f,%f",&x,&y,&z,&r,&w);
            ret->skeletons[i]->node[j].loc.x=x;
            ret->skeletons[i]->node[j].loc.y=y;
            ret->skeletons[i]->node[j].loc.z=z;
            ret->skeletons[i]->node[j].r=r;
            ret->skeletons[i]->node[j].w=w;
        }
    }
    fclose(fp);
    ret->fix();
    return ret;
}

IsHair* IsHairLoader::createTestIsHairObject(){

    IsHair* ret=new IsHair(3);
    ret->skeletons[0]=new IsHairSkeleton(3);
    ret->skeletons[1]=new IsHairSkeleton(3);
    ret->skeletons[2]=new IsHairSkeleton(3);
    ret->skeletons[0]->node[0].loc.x=0.5f;
    ret->skeletons[0]->node[0].loc.y=3.0f;
    ret->skeletons[0]->node[0].loc.z=8.0f;
    ret->skeletons[0]->node[0].r=0.4f;
    ret->skeletons[0]->node[0].w=0.5f;
    ret->skeletons[1]->node[0].loc.x=0.0f;
    ret->skeletons[1]->node[0].loc.y=3.0f;
    ret->skeletons[1]->node[0].loc.z=8.0f;
    ret->skeletons[1]->node[0].r=0.4f;
    ret->skeletons[1]->node[0].w=0.5f;
    ret->skeletons[2]->node[0].loc.x=-0.5f;
    ret->skeletons[2]->node[0].loc.y=3.0f;
    ret->skeletons[2]->node[0].loc.z=8.0f;
    ret->skeletons[2]->node[0].r=0.4f;
    ret->skeletons[2]->node[0].w=0.5f;

    ret->skeletons[0]->node[1].loc.x=0.5f;
    ret->skeletons[0]->node[1].loc.y=2.0f;
    ret->skeletons[0]->node[1].loc.z=6.5f;
    ret->skeletons[0]->node[1].r=1.0f;
    ret->skeletons[0]->node[1].w=1.0f;
    ret->skeletons[1]->node[1].loc.x=0.0f;
    ret->skeletons[1]->node[1].loc.y=2.0f;
    ret->skeletons[1]->node[1].loc.z=6.5f;
    ret->skeletons[1]->node[1].r=1.0f;
    ret->skeletons[1]->node[1].w=1.0f;
    ret->skeletons[2]->node[1].loc.x=-0.5f;
    ret->skeletons[2]->node[1].loc.y=2.0f;
    ret->skeletons[2]->node[1].loc.z=6.5f;
    ret->skeletons[2]->node[1].r=1.0f;
    ret->skeletons[2]->node[1].w=1.0f;

    ret->skeletons[0]->node[2].loc.x=0.6f;
    ret->skeletons[0]->node[2].loc.y=1.0f;
    ret->skeletons[0]->node[2].loc.z=5.0f;
    ret->skeletons[0]->node[2].r=0.7f;
    ret->skeletons[0]->node[2].w=0.75f;
    ret->skeletons[1]->node[2].loc.x=0.0f;
    ret->skeletons[1]->node[2].loc.y=1.0f;
    ret->skeletons[1]->node[2].loc.z=5.0f;
    ret->skeletons[1]->node[2].r=0.7f;
    ret->skeletons[1]->node[2].w=0.75f;
    ret->skeletons[2]->node[2].loc.x=-0.6f;
    ret->skeletons[2]->node[2].loc.y=1.0f;
    ret->skeletons[2]->node[2].loc.z=5.0f;
    ret->skeletons[2]->node[2].r=0.7f;
    ret->skeletons[2]->node[2].w=0.75f;

    ret->skeletons[0]->node[3].loc.x=0.75f;
    ret->skeletons[0]->node[3].loc.y=.0f;
    ret->skeletons[0]->node[3].loc.z=3.5f;
    ret->skeletons[0]->node[3].r=0.3f;
    ret->skeletons[0]->node[3].w=0.5f;
    ret->skeletons[1]->node[3].loc.x=0.0f;
    ret->skeletons[1]->node[3].loc.y=.0f;
    ret->skeletons[1]->node[3].loc.z=3.5f;
    ret->skeletons[1]->node[3].r=0.3f;
    ret->skeletons[1]->node[3].w=0.5f;
    ret->skeletons[2]->node[3].loc.x=-0.75f;
    ret->skeletons[2]->node[3].loc.y=.0f;
    ret->skeletons[2]->node[3].loc.z=3.5f;
    ret->skeletons[2]->node[3].r=0.3f;
    ret->skeletons[2]->node[3].w=0.5f;
    ret->threshold=0.5f;

    ret->fix();
    return ret;
}
