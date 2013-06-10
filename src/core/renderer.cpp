/**
 * @file renderer.cpp
 *
 * @author take
 */

#include "renderer.h"
#include "raytracer.h"
#include "vectorMath.h"
#include "brdf.h"
#include "paramdef.h"
#include <omp.h>

float SimpleRenderer::ambientOcclusion(RayIntersection& intersection){
    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=2.0f;
    ray.id=intersection.id;
    Triangle* pt=(Triangle*)intersection.id;
//    ray.start=intersection.point;
    pt->getPoint(intersection.k,intersection.l,ray.start);
    Vector4D normal;
    pt->getNormal(intersection.k,intersection.l,normal);
//    ray.start.x+=normal.x*0.0001;
//    ray.start.y+=normal.y*0.0001;
//    ray.start.z+=normal.z*0.0001;
    RayIntersection ri;
    float intensity=0.0f;
    int sample=100;
//    int sample=5;
    float sumPow=0.0f;
    for(int i=0;i<sample;i++){
//        ray.direct=normal;
        VectorMath::setRandomSphereVector(ray.direct);
        float expPow=VectorMath::dot(normal,ray.direct);
//        float expPow=1.0f;
//        printf("%f\n",expPow);
        if(expPow<0.0f){
            ray.direct.x*=-1.0f;
            ray.direct.y*=-1.0f;
            ray.direct.z*=-1.0f;
            expPow*=-1.0f;
        }
//        expPow=1.0;
        tracer.getIntersection(ray,ri);
        if(ri.detect){
//            intensity+=expPow*ri.depth/ray.maxDepth;
        }else{
            intensity+=expPow;
        }
        sumPow+=expPow;
    }
    intensity/=sumPow;
    return intensity;
}

void SimpleRenderer::simplePathTrace(RayIntersection& intersection,ColorRGBA& c){
//    int sample=3000;
    int sample=PATHTRACE_SUMPLE_NUM;
    ColorRGBA tmp;
    c.r=0.0f;
    c.g=0.0f;
    c.b=0.0f;
    for(int i=0;i<sample;i++){
        simplePathTraceSub(intersection,tmp,PATHTRACE_DEPTH_NUM);
        c.r+=tmp.r;
        c.g+=tmp.g;
        c.b+=tmp.b;
    }
    c.r/=sample;
    c.g/=sample;
    c.b/=sample;

}

void SimpleRenderer::simplePathTraceSub(RayIntersection& intersection,ColorRGBA& c,int depth){
    Triangle* pt=(Triangle*)intersection.id;
//    printf("%d",depth);
    Material* mate=pt->getMaterial();
    if(mate->emission){
        c.r=mate->emitPower;
        c.g=mate->emitPower;
        c.b=mate->emitPower;
        return;
    }

    if(depth<1){
        c.r=0.0f;
        c.g=0.0f;
        c.b=0.0f;
        return;
    }

    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.id=intersection.id;
    ray.ior=intersection.ior;
    pt->getPoint(intersection.k,intersection.l,ray.start);
    Vector4D normal;
    pt->getNormal(intersection.k,intersection.l,normal);
    RayIntersection ri;
    int vecMode=0;

    if(mate->reflection){
        vecMode=1;
    }else if(mate->refraction){
        vecMode=2;
    }

    if(vecMode==1){
        VectorMath::reflectionVector(intersection.ray,normal,ray.direct);
    }else if(vecMode==2){
//        float n1=intersection.ior;
        float n1=1.0f;
        float n2=mate->ior;
        int a=VectorMath::refractionVector(intersection.ray,normal,n1,n2,ray.direct);
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
        ColorRGBA inin;
        simplePathTraceSub(ri,inin,depth-1);

        if(vecMode!=0){
            c.r=inin.r;
            c.g=inin.g;
            c.b=inin.b;
        }else{
            ColorRGBA def;
            pt->brdf(intersection.k,intersection.l,ray.direct,intersection.ray,def);
            c.r=def.r*inin.r;
            c.g=def.g*inin.g;
            c.b=def.b*inin.b;
        }

        return;
    }else{
//        printf("-");
        IBackGround* bg=currentScene->getBackGround();
        if(bg){
            bg->getBGColor(ray.direct,c);
        }else{
            c.r=1.0f;
            c.g=1.0f;
            c.b=1.0f;
        }

        return;
    }

}

float SimpleRenderer::simpleDeffuse(RayIntersection& intersection){
    Triangle* pt=(Triangle*)intersection.id;
    Vector4D normal;
    pt->getNormal(intersection.k,intersection.l,normal);
    return (normal.z+1.0f)/2.0f;
}

void SimpleRenderer::render(Scene* scene,RawImage& img){
    int h=img.h;
    int w=img.w;
    float offx=w/2.0f;
    float offy=h/2.0f;
    float scl=offx>offy?offx:offy;
    currentScene=scene;
    tracer.setScene(scene);
    RayIntersection intersection;

#pragma omp parallel for private(intersection)// num_threads(6)
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int i=y*w+x;
            float fx=(x-offx)/scl;
            float fy=(y-offy)/scl;
            tracer.getIntersectionFromCamera(fx,fy,intersection);
            if(intersection.detect){
//                float intensity=ambientOcclusion(intersection);
//                float intensity=simpleDeffuse(intersection);
                img.pix[i].a=1.0f;
//                img.pix[i].r=intensity;
//                img.pix[i].g=intensity;
//                img.pix[i].b=intensity;
//                img.pix[i].r=intersection.point.x;
//                img.pix[i].g=intersection.point.y;
//                img.pix[i].b=intersection.point.z;

                simplePathTrace(intersection,img.pix[i]);

//                Vector4D v;
//                v.x=1.0f;
//                Material* mat=((Triangle*)(intersection.id))->getMaterial();
//                if(mat->emission){
//                    img.pix[i].r=1.0f;
//                    img.pix[i].g=1.0f;
//                    img.pix[i].b=1.0f;
//                }else{
//                    mat->brdf->function(v,v,v,v,img.pix[i]);
//                }
            }else{
                IBackGround* bg=currentScene->getBackGround();
                img.pix[i].a=1.0f;
                if(bg){
                    bg->getBGColor(intersection.ray,img.pix[i]);
                }else{
                    img.pix[i].r=0.0f;
                    img.pix[i].g=0.0f;
                    img.pix[i].b=0.0f;
                }


//                img.pix[i].r=0.0f;
//                img.pix[i].g=0.0f;
//                img.pix[i].b=0.0f;
            }
        }
        printf("y=%d\n",y);
    }
}
