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

struct Photon{
    ColorRGBA color;
    Vector4D direction;
    Vector4D point;
};

struct RayHitPoint{
    RayIntersection ri;
    int photonNum;
    ColorRGBA aphoton;
    float radius;
    int tempPhotonNum;
    ColorRGBA tempAphoton;
    int kdtIndex;
//    RayHitPoint(){}
//    RayHitPoint(const RayHitPoint& o){
//        printf("copy");
//    }
};

struct RayPhotonMap{
    RayHitPoint* ray;
    Photon photon;
};

class RayHitPointContainer{
private:
    RayHitPoint* hitpoints;
    RayHitPoint** sort[3];
    RayHitPoint** kdTree;
    int* divAxis;
    int* childrenCounter;
    int num;
    void heepup(RayHitPoint** array,int axis,int index);
    void heepdown(RayHitPoint** array,int axis,int index,int size);
    void sortPoint(RayHitPoint** array,int axis);
    void constractKdt(int index,int axis);
    void applyPhotonSub(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius,int index);
public:
    RayHitPointContainer(RayHitPoint* _hitpoints,int n);
    ~RayHitPointContainer();

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
private:
    SimpleRayTracer tracer;
    Scene* currentScene;
    LightSource* cLightSrc;
    RayHitPointContainer* rpc;
    void updateRadiance(RayPhotonMap* map,int num);
    void emitPhotonSub(RayIntersection& intersection,RayPhotonMap* map,int* offset,ColorRGBA& color,int depth);
    void emitPhoton(RayPhotonMap* map,int* offset);
    int raytraceSub(RayIntersection& intersection,RayIntersection& root,int depth);
    int raytrace(RayIntersection& intersection);
public:
    void render(Scene* scene,RawImage& img);
};


#endif /* PHOTON_H_ */
