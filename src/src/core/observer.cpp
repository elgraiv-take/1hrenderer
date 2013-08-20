/*
 * observer.cpp
 *
 *  Created on: 2013/08/20
 *      Author: Take
 */

#include "root.h"
#include "photon.h"
#include "main.h"
#include "imageIO.h"
#include "paramdef.h"

void createCurrentImage(RayHitPoint** screenRhp,RawImage* bg,RawImage* img,int photonNum){
    int w=img->w;
    int h=img->h;
    for(int i=0;i<w*h;i++){
        if(screenRhp[i]){
            float rate=PPM_AREA(screenRhp[i]->radius)*photonNum;
            img->pix[i].r=screenRhp[i]->aphoton.r/rate;
            img->pix[i].g=screenRhp[i]->aphoton.g/rate;
            img->pix[i].b=screenRhp[i]->aphoton.b/rate;
            img->pix[i].r=img->pix[i].r<0.0f?0.0f:img->pix[i].r;
            img->pix[i].g=img->pix[i].g<0.0f?0.0f:img->pix[i].g;
            img->pix[i].b=img->pix[i].b<0.0f?0.0f:img->pix[i].b;
        }else{
            img->pix[i]=bg->pix[i];
        }
    }
}

void observe(LPVOID params){
//    printf("-----");
    ThreadParams* tp=(ThreadParams*)params;

    RawImage* img=new RawImage(tp->w,tp->h);
    RawImage* bg;
    RayHitPoint** screenRhp;
    int photonNum;
    int start=tp->start;
    int stepNum=DEAD_LINE/OUTPUT_TIME_STEP;
    char outputFileName[512];

    for(int i=1;i<stepNum;i++){
        clock_t current=clock();
        int next=i*OUTPUT_TIME_STEP-SAFETY_MARGIN;
        int sleepTime=MAX(next-(current-tp->start),1);
        Sleep(sleepTime);
        screenRhp=tp->renderer->getCurrent(&photonNum,&bg);
        if(screenRhp){
            createCurrentImage(screenRhp,bg,img,photonNum);
            ImageIO::gammaCorrection((*img),2.20f);
            sprintf(outputFileName,"temp_%03d.ppm",i);
            ImageIO::writePPM(outputFileName,(*img));
            printf("------%s\n",outputFileName);
        }
    }
    clock_t current=clock();
    int sleepTime=MAX(DEAD_LINE-(current-tp->start)-SAFETY_MARGIN,1);
    Sleep(sleepTime);
    tp->renderer->stopEmission();

}

