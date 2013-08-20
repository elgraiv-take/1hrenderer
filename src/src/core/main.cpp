/**
 * @file main.cpp
 *
 * @author take
 */

#include "root.h"
#include "largeArray.h"
#include "renderer.h"
#include "test.h"
#include "vectorMath.h"
#include "imageIO.h"
#include "photon.h"
#include "main.h"
#include <float.h>
#include <crtdbg.h>

void startObserverThread(PPMRenderer* renderer,int w,int h,clock_t start){
    DWORD dwThreadID;
    ThreadParams* param=new ThreadParams();
    param->renderer=renderer;
    param->w=w;
    param->h=h;
    param->start=start;
    HANDLE hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)observe,param,0,&dwThreadID);
}

int testRender(char* outFile){
    clock_t start=clock();
    Scene* sc=getTestScene();
//    SimpleRenderer renderer;
    PPMRenderer renderer;
//    SPPMRenderer renderer;
    RawImage img(1280,960);
//    RawImage img(1920,1080);
//    RawImage img(1280*2,960*2);
//    RawImage img(800,600);
//    RawImage img(640,480);
//    RawImage img(400,300);
//    RawImage img(200,150);
//    RawImage img(128,96);
    startObserverThread(&renderer,img.w,img.h,start);
    renderer.render(sc,img);
    char* f;
    if(outFile){
        f=outFile;
    }else{
        f="test.ppm";
    }
    ImageIO::gammaCorrection(img,2.20f);
    int res=ImageIO::writePPM(f,img);
    SAFE_DELETE_O(sc);
    clock_t end=clock();
    printf("rendering time :%d.%03ds\n",(end-start)/1000,(end-start)%1000);
    return res;
}


/**
 * @brief ���C���֐�
 *
 * @param argc argv�̗v�f��
 * @param argv [0]:�v���O����,[1]�ȍ~����
 * @return ���s����
 * @retval 0 ����I��
 * @retval 0�ȊO �ُ�I��
 */
int main(int argc,char **argv){
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//    memtest();
    if(argc>1){
        testRender(argv[1]);
    }else{
        testRender(NULL);
    }

    return 0;
}
