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


float SimpleRenderer::simpleDeffuse(RayIntersection& intersection){
    Vector4D normal;
    if(intersection.type==IS_HAIR){
        IsHair* ish=(IsHair*)intersection.id;
        ish->getNormal(intersection,normal);
    }else{
        Triangle* pt=(Triangle*)intersection.id;
        pt->getNormal(intersection.k,intersection.l,normal);
    }
    return (normal.z+1.0f)/2.0f;
}
void SimpleRenderer::normal(RayIntersection& intersection,ColorRGBA& ret){
    Vector4D normal;
//    if(intersection.type==IS_HAIR){
//        IsHair* ish=(IsHair*)intersection.id;
//        ish->getNormal(intersection,normal);
//    }else{
//        Triangle* pt=(Triangle*)intersection.id;
//        pt->getNormal(intersection.k,intersection.l,normal);
//    }
    intersection.id->getNormal(intersection,normal);
    ret.r=(normal.x+1.0f)*0.5;
    ret.g=(normal.y+1.0f)*0.5;
    ret.b=(normal.z+1.0f)*0.5;
}

void SimpleRenderer::tangent(RayIntersection& intersection,ColorRGBA& ret){
    Vector4D tangent;
    intersection.id->getTangent(intersection,tangent);
    ret.r=(tangent.x+1.0f)*0.5;
    ret.g=(tangent.y+1.0f)*0.5;
    ret.b=(tangent.z+1.0f)*0.5;
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

                float intensity=simpleDeffuse(intersection);
                img.pix[i].a=1.0f;
                img.pix[i].r=intensity;
                img.pix[i].g=intensity;
                img.pix[i].b=intensity;
//                normal(intersection,img.pix[i]);
                tangent(intersection,img.pix[i]);

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

            }
        }
        printf("y=%d\n",y);
    }
}
