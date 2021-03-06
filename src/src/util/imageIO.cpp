/**
 * @file imageIO.cpp
 *
 * @author take
 */

#include "root.h"
#include "imageIO.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

void ImageIO::gammaCorrection(RawImage& img,float gamma){
    for(int i=0;i<img.w*img.h;i++){
        img.pix[i].r=pow(img.pix[i].r,1/gamma);
        img.pix[i].g=pow(img.pix[i].g,1/gamma);
        img.pix[i].b=pow(img.pix[i].b,1/gamma);
    }
}

int ImageIO::ccFtoI(float c){
    int ic=(int)(c*255);
    if(ic<0){
        ic=0;
    }
    if(ic>255){
        ic=255;
    }
    return ic;
}

int ImageIO::writePPM(const char* fileName,const RawImage& img){
    FILE* fp=fopen(fileName,"w");
    if(!fp){
        return -1;
    }
    int w=img.w;
    int h=img.h;
    fprintf(fp,"P3\n#renderer test\n%d %d\n255\n",w,h);

    for(int y=0,i=0;y<h;y++){
        for(int x=0;x<w;x++,i++){
            int r=ccFtoI(img.pix[i].r*img.pix[i].a);
            int g=ccFtoI(img.pix[i].g*img.pix[i].a);
            int b=ccFtoI(img.pix[i].b*img.pix[i].a);
            fprintf(fp,"%d %d %d ",r,g,b);
        }
    }
    fflush(fp);
    fclose(fp);
    return 0;
}

int ImageIO::readBRDF(const char* fileName,RawImage** img){
    FILE* fp=fopen(fileName,"r");
    if(!fp){
        return -1;
    }
    RawImage* retimg=new RawImage(181,361);
    int the,phy;
    float r,g,b;
    for(int y=0,i=0;y<361;y++){
        for(int x=0;x<181;x++,i++){
            fscanf(fp,"%d,%d,%f,%f,%f",&the,&phy,&r,&g,&b);
            retimg->pix[i].r=r;
            retimg->pix[i].g=g;
            retimg->pix[i].b=b;
        }
    }
    fclose(fp);
    (*img)=retimg;
    return 0;
}

int ImageIO::readRawData(const char* fileName,RawImage** img){
    FILE* fp=fopen(fileName,"rb");
    if(!fp){
        return -1;
    }

    int wh[2];
    fread(wh,sizeof(int),2,fp);
    printf("tex%d-%d\n",wh[0],wh[1]);
    RawImage* ret=new RawImage(wh[0],wh[1]);
    fread(ret->pix,sizeof(ColorRGBA),wh[0]*wh[1],fp);
    fclose(fp);
    (*img)=ret;
    return 0;
}

int ImageIO::readLine(const char* buff,int off,char* str,int len){
    for(int i=0,ii=off;i<len;i++,ii++){
        if(buff[ii]=='\n'){
            str[i]='\0';
            return ii+1;
        }
        str[i]=buff[ii];
    }
    return off+len;
}

int ImageIO::readHDR(const char* fileName,RawImage** img){
    FILE* fp=fopen(fileName,"rb");
    if(!fp){
        return -1;
    }
    struct _stat stbuf;
    int res=_stat(fileName,&stbuf);
    if(res){
        fclose(fp);
        return -1;
    }
    int fsize=stbuf.st_size;
    printf("%d\n",fsize);

    char* fbuff=new char[fsize];

    fread(fbuff,sizeof(char),fsize,fp);
    fclose(fp);
    static const int BUFF_LEN=512;
    char strbuff[BUFF_LEN];

    int offset=0;
    int w=-1,h=-1;
    while(offset<fsize){
        int rlen=readLine(fbuff,offset,strbuff,BUFF_LEN);
        offset=rlen;
        printf("%s\n",strbuff);
        if(sscanf(strbuff,"-Y %d +X %d",&h,&w)==2){
            break;
        }
    }

    if(w<0||h<0){
        SAFE_DELETE_A(fbuff);

        return -1;
    }
    RawImage* retimg=new RawImage(w,h);
    unsigned char* rowbuff=new unsigned char[w*4];
    int buffi=offset;
    for(int y=0;y<h;y++){
        int i=0;
        buffi+=4;
        while(i<w*4){
            int b=0xff&(int)fbuff[buffi];

            buffi++;
            if(b>128){
                int len=b-128;
                int nxt=0xff&(int)fbuff[buffi];
                buffi++;
                for(int ii=0;ii<len;ii++,i++){
                    rowbuff[i]=(char)nxt;

                }
            }else{
                int len=b;
                for(int ii=0;ii<len;ii++,i++){
                    rowbuff[i]=0xff&(int)fbuff[buffi];
                    buffi++;
                }
            }
        }

        for(int x=0;x<w;x++){
            retimg->pix[(y*w+x)].r=(float)(rowbuff[x]*pow(2.0f,rowbuff[x+3*w]-128));
            retimg->pix[(y*w+x)].g=(float)(rowbuff[x+w]*pow(2.0f,rowbuff[x+3*w]-128));
            retimg->pix[(y*w+x)].b=(float)(rowbuff[x+2*w]*pow(2.0f,rowbuff[x+3*w]-128));
            retimg->pix[(y*w+x)].r=pow(1+retimg->pix[(y*w+x)].r,1/2.2f);
            retimg->pix[(y*w+x)].g=pow(1+retimg->pix[(y*w+x)].g,1/2.2f);
            retimg->pix[(y*w+x)].b=pow(1+retimg->pix[(y*w+x)].b,1/2.2f);
        }
        printf("y=%d\n",y);
    }

    (*img)=retimg;

    SAFE_DELETE_A(fbuff);
    SAFE_DELETE_A(rowbuff);
    return 0;

}
