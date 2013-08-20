/**
 * @file background.cpp
 *
 * @author take
 */

#include "background.h"
#include "vectorMath.h"

void SphereImageBG::getBGColor(Vector4D& vec,ColorRGBA& bgc){
    Vector4D v=vec;
    VectorMath::normalize(v);
    float ph=asin(v.z);
    float x=v.x;
    float y=v.y;
    float len=sqrt(x*x+y*y);
    if(len<FLOAT_MIN_VAL){
        x=1.0f;
        y=0.0f;
    }else{
        x/=len;
        y/=len;
    }
    float th=acos(x);
    if(y<0.0f){
        th=(2.0f*M_PI)-th;
    }

//    th+=3.0f*M_PI/2.0f;
    th+=M_PI;
    if(th>2.0f*M_PI){
        th-=2.0f*M_PI;
    }

    float vf=0.5f-ph/M_PI;
    float uf=1.0f-th/(2.0f*M_PI);

//    printf("%f\n",vf);
    if(vf<0.0f){
        vf=0.0f;
    }
    if(vf>1.0f){
        vf=1.0f;
    }
    if(uf<0.0f){
        uf=0.0f;
    }
    if(uf>1.0f){
        uf=1.0f;
    }

    int ui=(int)(uf*((bgi->w)-1));
    int vi=(int)(vf*((bgi->h)-1));

    int index=bgi->w*vi+ui;
    bgc.r=bgi->pix[index].r*pow;
    bgc.g=bgi->pix[index].g*pow;
    bgc.b=bgi->pix[index].b*pow;
}
SphereImageBG::SphereImageBG(){
    bgi=NULL;
}


SphereImageBG::~SphereImageBG(){
    SAFE_DELETE_O(bgi);
}

void SphereImageBG::setImage(RawImage* _bgi){
    SAFE_DELETE_O(bgi);
    bgi=_bgi;
}
void SphereImageBG::setPower(float _pow){
    pow=_pow;
}

void ConstColorBG::setColor(ColorRGBA& _color){
    color=_color;
}
void ConstColorBG::setPower(float _pow){
    pow=_pow;
}
void ConstColorBG::getBGColor(Vector4D& vec,ColorRGBA& bgc){
    bgc=color;
}
