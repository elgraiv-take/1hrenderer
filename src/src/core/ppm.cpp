/**
 * @file ppm.cpp
 *
 * @author take
 */

#include "renderer.h"
#include "raytracer.h"
#include "vectorMath.h"
#include "brdf.h"
#include "paramdef.h"
#include <omp.h>
#include "photon.h"

inline float mbfunc2(float r){
    if(r>1){
        return 0.0f;
    }
    if(r>(1.0f/3.0f)){
        float imx=1.0f-r;
        return 1.5f*imx*imx;
    }
    return 1.0f-3.0f*r*r;
}

int PPMRenderer::raytraceSub(RayIntersection& intersection,RayIntersection& root,int depth){
    Renderable* pt=intersection.id;
    //    printf("%d",depth);
    Material* mate=pt->getMaterial();
    if(mate->emission){
        root=intersection;
        return LIGHTSOURCE;
    }

    if(depth<1){
        return NODITECTION;
    }

    int vecMode=0;

    if(mate->reflection){
        vecMode=1;
    }else if(mate->refraction){
        vecMode=2;
    }else{
        root=intersection;
        return DIFFUSE;
    }

    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.id=intersection.id;
    ray.ior=intersection.ior;
    pt->getPoint(intersection,ray.start);
    Vector4D normal;
    int in=pt->getNormal(intersection,normal);
    RayIntersection ri;

    if(vecMode==1){
        VectorMath::reflectionVector(intersection.ray,normal,ray.direct);
    }else if(vecMode==2){
        //        float n1=intersection.ior;
        float n1=1.0f;
        float n2=mate->ior;
        int a=VectorMath::refractionVector(intersection.ray,normal,in,n1,n2,ray.direct);
        if(a){
        }else{
            ray.ior=mate->ior;
        }
    }

    tracer.getIntersection(ray,ri);
    if(ri.detect){
        return raytraceSub(ri,root,depth-1);
    }else{
        root=ri;
        return BACKGROUND;
    }
}
int PPMRenderer::raytrace(RayIntersection& intersection){
    return raytraceSub(intersection,intersection,PPM_MAX_RAY_DEPTH);
}

//void PPMRenderer::finalGather(RayHitPoint& rayhp){
//    RayObject ray;
//    ray.minDepth=0.0f;
//    ray.maxDepth=2.0f;
//    ray.id=rayhp.ri.id;
//    Triangle* pt=(Triangle*)rayhp.ri.id;
//    pt->getPoint(rayhp.ri.k,rayhp.ri.l,ray.start);
//    Vector4D normal;
//    pt->getNormal(rayhp.ri.k,rayhp.ri.l,normal);
//    RayIntersection ri;
//    float intensity=0.0f;
//    for(int i=0;i<PPM_FINAL_GATHER_NUM;i++){
//        VectorMath::setRandomSphereVector(ray.direct);
//        float expPow=VectorMath::dot(normal,ray.direct);
//        if(expPow<0.0f){
//            ray.direct.x*=-1.0f;
//            ray.direct.y*=-1.0f;
//            ray.direct.z*=-1.0f;
//            expPow*=-1.0f;
//        }
//        tracer.getIntersection(ray,ri);
//        if(ri.detect){
//            int res=raytrace(ri);
//            if(res==LIGHTSOURCE){
//                float ep=((Triangle*)ri.id)->getMaterial()->emitPower;
//                rayhp.tempPhotonNum+=1.0f;
//                rayhp.tempAphoton.r+=ep;
//                rayhp.tempAphoton.g+=ep;
//                rayhp.tempAphoton.b+=ep;
//            }
//        }
//    }
//    if(rayhp.tempPhotonNum){
//        PPMUtil::updateRadiance1R(&rayhp);
//    }
//
//}

void PPMRenderer::raytracePass(int w,int h,RayIntersection* intersection,int* pixtype){
    float offx=w/2.0f;
    float offy=h/2.0f;
    float scl=offx>offy?offx:offy;
#pragma omp parallel for// num_threads(6)
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int i=y*w+x;
            float fx=(x-offx)/scl;
            float fy=(y-offy)/scl;
            tracer.getIntersectionFromCamera(fx,fy,intersection[i]);
            if(intersection[i].detect){
                pixtype[i]=raytrace(intersection[i]);
                if(intersection[i].detect){
                    Renderable* pt=intersection[i].id;
                    pt->getPoint(intersection[i],intersection[i].point);
                }
            }else{
                pixtype[i]=BACKGROUND;
            }
        }
//        printf("y=%d\n",y);
    }
}

void PPMRenderer::photonmappingPass(int w,int h,RayIntersection* intersection,int* pixtype,ColorRGBA* pix){

    thnum=omp_get_max_threads();
//    int thnum=1;
    omp_set_num_threads(thnum);
    printf("num threads :%d\n",thnum);
    int hitcount=0;
    for(int i=0;i<h*w;i++){
        if(pixtype[i]==DIFFUSE){
            hitcount++;
        }
    }
    tempImg=new RawImage(w,h);
    RayHitPoint* rhp=new RayHitPoint[hitcount];
    screenRhp=new RayHitPoint*[w*h];
    hitcount=0;
    for(int i=0;i<h*w;i++){
        if(pixtype[i]==DIFFUSE){
            rhp[hitcount].ri=intersection[i];
            rhp[hitcount].photonNum=0;
            rhp[hitcount].tempPhotonNum=0;
            rhp[hitcount].radius=PPM_DEFAULT_RADIUS;
            intersection[i].id->getNormal(intersection[i],rhp[hitcount].normal);
            intersection[i].id->getTangent(intersection[i],rhp[hitcount].tangent);
            screenRhp[i]=(&(rhp[hitcount]));
            hitcount++;

        }else{
            screenRhp[i]=NULL;
            if(pixtype[i]==NODITECTION){
                pix[i].r=0.0f;
                pix[i].g=0.0f;
                pix[i].b=0.0f;
            }else if(pixtype[i]==LIGHTSOURCE){
                float ep=intersection[i].id->getMaterial()->emitPower;
                pix[i].r=ep*intersection[i].id->getMaterial()->emitColor.r;
                pix[i].g=ep*intersection[i].id->getMaterial()->emitColor.r;
                pix[i].b=ep*intersection[i].id->getMaterial()->emitColor.r;
            }else if(pixtype[i]==BACKGROUND){
                IBackGround* bg=currentScene->getBackGround();
                if(bg){
                    bg->getBGColor(intersection->ray,pix[i]);
                }else{
                    pix[i].r=0.0f;
                    pix[i].g=0.0f;
                    pix[i].b=0.0f;
                }
            }
            tempImg->pix[i]=pix[i];
        }
    }
    rpc=new RayHitPointContainer(rhp,hitcount);
    rpc->updateMaxRadiusAll();
    RayPhotonMap** rpms=new RayPhotonMap*[thnum];
    int* mapCount=new int[thnum];
    photonCount=new int[thnum];
    for(int i=0;i<thnum;i++){
        rpms[i]=new RayPhotonMap[MAP_SIZE];
        mapCount[i]=0;
        photonCount[i]=0;
    }

    printf("emit photon\n");
#pragma omp parallel for
    for(int i=0;i<PPM_PHOTON_NUM;i++){
        if(!enableEmission){
            continue;
        }

        int thNo=omp_get_thread_num();
        photonCount[thNo]++;
        emitPhoton(rpms[thNo],&(mapCount[thNo]));
        if(i%1000==0){
            printf("%d %d\n",i,mapCount[thNo]);
        }
        if(mapCount[thNo]>APPLY_SIZE){
#pragma omp critical (update)
            {
                updateRadiance(rpms[thNo],mapCount[thNo]);
            }
            mapCount[thNo]=0;
        }
    }
    int photonNum=0;
    for(int i=0;i<thnum;i++){
        photonNum+=photonCount[i];
        updateRadiance(rpms[i],mapCount[i]);
    }
    SAFE_DELETE_A(mapCount);
    SAFE_DELETE_A(photonCount);
    printf("f_photon_%d\n",photonNum);

    float maxRad=0.0f;
    for(int i=0;i<w*h;i++){
        if(pixtype[i]==DIFFUSE){
            if(maxRad<screenRhp[i]->radius){
                maxRad=screenRhp[i]->radius;
            }
            float rate=PPM_AREA(screenRhp[i]->radius)*photonNum;
            pix[i].r=screenRhp[i]->aphoton.r/rate;
            pix[i].g=screenRhp[i]->aphoton.g/rate;
            pix[i].b=screenRhp[i]->aphoton.b/rate;
            pix[i].r=pix[i].r<0.0f?0.0f:pix[i].r;
            pix[i].g=pix[i].g<0.0f?0.0f:pix[i].g;
            pix[i].b=pix[i].b<0.0f?0.0f:pix[i].b;
//            pix[i].r=pow(pix[i].r,1/2.20f);
//            pix[i].g=pow(pix[i].g,1/2.20f);
//            pix[i].b=pow(pix[i].b,1/2.20f);
        }
    }
    printf("max-r:%f\n",maxRad);
    SAFE_DELETE_O(rpc);
    SAFE_DELETE_A(rhp);
    SAFE_DELETE_A(screenRhp);
    for(int i=0;i<thnum;i++){
        SAFE_DELETE_A(rpms[i]);
    }
    SAFE_DELETE_A(rpms);
    SAFE_DELETE_O(tempImg);
}

RayHitPoint** PPMRenderer::getCurrent(int* photonNum,RawImage** bg){
    if(screenRhp&&photonCount&&tempImg){
        int pnum=0;
        for(int i=0;i<thnum;i++){
            pnum+=photonCount[i];
        }
        (*photonNum)=pnum;
        (*bg)=tempImg;
        return screenRhp;
    }
    return NULL;
}

void PPMRenderer::render(Scene* scene,RawImage& img){
    int h=img.h;
    int w=img.w;

    currentScene=scene;
    tracer.setScene(scene);
    cLightSrc=new LightSource(scene);
    RayIntersection* intersection=new RayIntersection[h*w];
    int* pixtype=new int[h*w];

    raytracePass(w,h,intersection,pixtype);
    photonmappingPass(w,h,intersection,pixtype,img.pix);
    SAFE_DELETE_A(intersection);
    SAFE_DELETE_A(pixtype);
    SAFE_DELETE_O(cLightSrc);
}
void PPMRenderer::emitPhotonSub(RayIntersection& intersection,RayPhotonMap* map,int* offset,ColorRGBA& color,int depth){
    Renderable* pt=intersection.id;
    Material* mate=pt->getMaterial();
    if(mate->emission){
//        printf("-----------------------------\n");
        return;
    }
    int vecMode=0;
    if(mate->reflection){
        vecMode=1;
    }else if(mate->refraction){
        vecMode=2;
    }else{
        Photon tmp;
        tmp.color=color;
        tmp.direction=intersection.ray;
        pt->getPoint(intersection,tmp.point);
        rpc->applyPhoton(map,tmp,offset,MAP_SIZE,PPM_DEFAULT_RADIUS);
    }
    if(depth<1){
        return;
    }

    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.id=intersection.id;
    ray.ior=intersection.ior;
    pt->getPoint(intersection,ray.start);
    Vector4D normal;
    int in=pt->getNormal(intersection,normal);
    RayIntersection ri;

    if(vecMode==1){
        VectorMath::reflectionVector(intersection.ray,normal,ray.direct);
    }else if(vecMode==2){
        float n1=1.0f;
        float n2=mate->ior;
        int a=VectorMath::refractionVector(intersection.ray,normal,in,n1,n2,ray.direct);
        if(a){
        }else{
            ray.ior=mate->ior;
        }
    }else{
        VectorMath::setRandomSphereVector(ray.direct);
        float expPow=VectorMath::dot(normal,ray.direct);
        if(expPow<0.0f){
            ray.direct.x*=-1.0f;
            ray.direct.y*=-1.0f;
            ray.direct.z*=-1.0f;
        }
    }

    tracer.getIntersection(ray,ri);
    if(ri.detect){
        ColorRGBA c;
        if(vecMode!=0){
            c.r=color.r;
            c.g=color.g;
            c.b=color.b;
        }else{
            ColorRGBA def;
            Vector4D tan,normal;
            pt->getNormal(intersection,normal);
            pt->getTangent(intersection,tan);
            pt->getMaterial()->brdf->function(intersection,normal,ray.direct,intersection.ray,tan,def);
//            pt->brdf(intersection.k,intersection.l,ray.direct,intersection.ray,def);
            c.r=def.r*color.r;
            c.g=def.g*color.g;
            c.b=def.b*color.b;
        }
        emitPhotonSub(ri,map,offset,c,depth-1);

    }
}
void PPMRenderer::emitPhoton(RayPhotonMap* map,int* offset){
    RayObject ray;
    RayIntersection ri;
    ColorRGBA light;
    cLightSrc->getRandumPhoton(ray,light);
    tracer.getIntersection(ray,ri);
    if(ri.detect){
        emitPhotonSub(ri,map,offset,light,PPM_MAX_PHOTON_DEPTH);
    }
}

void PPMRenderer::stopEmission(){
    enableEmission=0;
}

void PPMUtil::updateRadiance1R(RayHitPoint* ray){
    float n=ray->photonNum;
    float m=ray->tempPhotonNum;
    float namnm=((float)n+PPM_ALPHA*m)/(float)(n+m);
    float newR=ray->radius*sqrt(namnm);
    ray->radius=newR;
    float nr=(ray->aphoton.r+ray->tempAphoton.r)*namnm;
    float ng=(ray->aphoton.g+ray->tempAphoton.g)*namnm;
    float nb=(ray->aphoton.b+ray->tempAphoton.b)*namnm;
    //            printf("%d\n",ray->photonNum);
    ray->aphoton.r=nr;
    ray->aphoton.g=ng;
    ray->aphoton.b=nb;
    //            printf("%f\n",ray->aphoton.r);
    ray->tempAphoton.r=0.0f;
    ray->tempAphoton.g=0.0f;
    ray->tempAphoton.b=0.0f;
    ray->photonNum=n+m;
    ray->tempPhotonNum=0;
    //            printf("------%x\n",ray);
}

void PPMRenderer::updateRadiance(RayPhotonMap* map,int num){
    ColorRGBA tc;
//    Vector4D normal,tan;
    for(int i=0;i<num;i++){
        RayHitPoint* ray=map[i].ray;
        if(map[i].r<ray->radius){
            float weight=PPM_WEIGHT(ray->radius,map[i].r);

//            ray->ri.id->getNormal(ray->ri,normal);
            ray->ri.id->getMaterial()->brdf->function(ray->ri,ray->normal,map[i].photon.direction,ray->ri.ray,ray->tangent,tc);
//            ((Triangle*)(ray->ri.id))->brdf(ray->ri.k,ray->ri.l,map[i].photon.direction,ray->ri.ray,tc);
//            printf("%f\n",tc.r);
            if(!(map[i].photon.color.r>=0.0f)){
                printf("%f\n",map[i].photon.color.r); //TODO
                continue;
            }
            ray->tempPhotonNum+=weight;
            ray->tempAphoton.r+=(tc.r*map[i].photon.color.r*weight);
            ray->tempAphoton.g+=(tc.g*map[i].photon.color.g*weight);
            ray->tempAphoton.b+=(tc.b*map[i].photon.color.b*weight);
        }
    }
    for(int i=0;i<num;i++){
        RayHitPoint* ray=map[i].ray;
        if(ray->tempPhotonNum){
            PPMUtil::updateRadiance1R(ray);
            rpc->updateMaxRadius(ray->kdtIndex);
        }
    }
}

//----------------------------------------------------------------

//------------------------------------------------------------------------------------

