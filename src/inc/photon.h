/**
 * @file photon.h
 *
 * @author take
 */

#ifndef PHOTON_H_
#define PHOTON_H_

#include "structDef.h"
#include "renderer.h"

#define AXIS_X (0)
#define AXIS_Y (1)
#define AXIS_Z (2)
#define NEXT_AXIS(a) ((a+1)%3)


#define DIFFUSE 0
#define BACKGROUND 1
#define LIGHTSOURCE 2
#define NODITECTION 3

struct Photon{
    ColorRGBA color;
    Vector4D direction;
    Vector4D point;
};

struct RayHitPoint{
    RayIntersection ri;
    Vector4D normal;
    Vector4D tangent;
    float photonNum;
    ColorRGBA aphoton;
    float radius;
    float tempPhotonNum;
    ColorRGBA tempAphoton;
    int kdtIndex;
};

struct RayPhotonMap{
    RayHitPoint* ray;
    Photon photon;
    float r;
};

class RayHitPointContainer{
private:
    RayHitPoint* hitpoints;
    RayHitPoint** sort[3];
    RayHitPoint** kdTree;
    int* divAxis;
    int* childrenCounter;
    float* maxRadius;
    int num;
    void heepup(RayHitPoint** array,int axis,int index);
    void heepdown(RayHitPoint** array,int axis,int index,int size);
    void sortPoint(RayHitPoint** array,int axis);
    void constractKdt(int index,int axis);

    void applyPhotonSub(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius,int index);
    public:
    RayHitPointContainer(RayHitPoint* _hitpoints,int n);
    ~RayHitPointContainer();
    void updateMaxRadiusAll();
    void updateMaxRadius(int index);
    void applyPhoton(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius);
};

class LightSource{
private:
    Triangle** sources;
    float* areas;
    int num;
    public:
    LightSource(Scene* scene);
    ~LightSource();
    void getRandumPhoton(RayObject& ray,ColorRGBA& color);
};

class PPMRenderer:public IRenderer{
protected:
    BVHRayTracer tracer;
    Scene* currentScene;
    LightSource* cLightSrc;
    RayHitPointContainer* rpc;
    int enableEmission;
    RayHitPoint** screenRhp;
    RawImage* tempImg;
    int* photonCount;
    int thnum;

private:
    void updateRadiance(RayPhotonMap* map,int num);
    void emitPhotonSub(RayIntersection& intersection,RayPhotonMap* map,int* offset,ColorRGBA& color,int depth);
    int raytraceSub(RayIntersection& intersection,RayIntersection& root,int depth);

protected:
    void emitPhoton(RayPhotonMap* map,int* offset);
    int raytrace(RayIntersection& intersection);
    virtual void raytracePass(int w,int h,RayIntersection* intersection,int* pixtype);
    virtual void photonmappingPass(int w,int h,RayIntersection* intersection,int* pixtype,ColorRGBA* pix);

public:
    virtual void render(Scene* scene,RawImage& img);
    PPMRenderer(){
        thnum=0;
        photonCount=NULL;
        enableEmission=1;
        cLightSrc=NULL;
        currentScene=NULL;
        rpc=NULL;
        screenRhp=NULL;
        tempImg=NULL;
    }
    void stopEmission();
    RayHitPoint** getCurrent(int* photonNum,RawImage** bg);
};

class SPPMRenderer:public PPMRenderer{
protected:
    void raytracePass(int w,int h,RayIntersection* intersection,int* pixtype);
public:
    void render(Scene* scene,RawImage& img);
};

namespace PPMUtil {
void updateRadiance1R(RayHitPoint* ray);
}
;


//#define PPM_WF_LINER
//#define PPM_WF_QUADRATIC

#if defined(PPM_WF_LINER)
#define PPM_WEIGHT(r,d) (1.0f-d/r)
#define PPM_AREA(r) ((r*r*M_PI)/3.0f)
#elif defined(PPM_WF_QUADRATIC)
#define PPM_WEIGHT(r,d) (mbfunc2(d/r))
#define PPM_AREA(r) ((r*r*M_PI*7.0f)/36.0f)
#else
#define PPM_WEIGHT(r,d) (1.0f)
#define PPM_AREA(r) (r*r*M_PI)
#endif


#endif /* PHOTON_H_ */
