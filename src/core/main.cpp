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
#include <time.h>
#include <float.h>

struct Test{
    int o[100];
};
void memtest(){
    int n=1;
    for(int i=0;i<10;i++){
        LargeArray<Test>* array=new LargeArray<Test>(n);
//        Test* array=new Test[n];
        printf("ok-%d %d\n",n,i);
        getchar();
        delete array;
//        delete[] array;
        n*=10;
    }
}

int testRender(char* outFile){
    clock_t start=clock();
    Scene* sc=getTestScene();
    SimpleRenderer renderer;
//    RawImage img(1280,960);
//    RawImage img(640,480);
    RawImage img(400,300);
//    RawImage img(200,150);
//    RawImage img(128,96);
    renderer.render(sc,img);
    char* f;
    if(outFile){
        f=outFile;
    }else{
        f="test.ppm";
    }
    int res=ImageIO::writePPM(f,img);
    SAFE_DELETE_O(sc);
    clock_t end=clock();
    printf("rendering time :%d.%03ds\n",(end-start)/1000,(end-start)%1000);
    return res;
}

void randTest(){

    float sum=0.0f;
    int sumple=100000;
    for(int i=0;i<sumple;i++){
        sum+=Xorshift::nextf();
    }
    sum/=sumple;
    printf("%f\n",sum);

}

void hdrTest(){
    RawImage* img;
    int res=ImageIO::readHDR("D:/workspace/eclipseWS2013/girender/data/bg.hdr",&img);
    if(res){
        return;
    }
    res=ImageIO::writePPM("d:/tmpOut/test.ppm",*img);
}

/**
 * @brief メイン関数
 *
 * @param argc argvの要素数
 * @param argv [0]:プログラム,[1]以降未定
 * @return 実行結果
 * @retval 0 正常終了
 * @retval 0以外 異常終了
 */
int main(int argc,char **argv){
//    memtest();
    if(argc>1){
        testRender(argv[1]);
    }else{
        testRender(NULL);
    }

//    float a=1.0f;
//    for(int i=0;i<1000;i++){
//        a*=10;
//        printf("%f\n",a);
//        if(a>FLT_MAX){
//            printf("end\n");
//            break;
//
//        }
//    }

//    hdrTest();
//    randTest();
//    printf("%f",VectorMath::det(3,2,3,4,5,6,7,8,10));
    return 0;
}
