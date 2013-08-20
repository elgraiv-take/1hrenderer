/**
 * @file structImplement.cpp
 *
 * @author take
 */

#include "structDef.h"
#include "brdf.h"

Vector4D::Vector4D(){
    x=y=z=0;
    w=1;
}

Vector4D::Vector4D(float _x,float _y,float _z,float _w){
    x=_x;
    y=_y;
    z=_z;
    w=_w;
}
Vector4D::Vector4D(float _x,float _y,float _z){
    x=_x;
    y=_y;
    z=_z;
    w=0.0f;
}

Vector4D::Vector4D(const float initValues[]){
    xyzw[0]=initValues[0];
    xyzw[1]=initValues[1];
    xyzw[2]=initValues[2];
    xyzw[3]=initValues[3];
}

Matrix4D::Matrix4D(){
    a11=a22=a33=a44=1;
    a12=a13=a14=0;
    a21=a23=a24=0;
    a31=a32=a34=0;
    a41=a42=a43=0;
}
Matrix4D::Matrix4D(const float initValues[]){
    a[0]=initValues[0];
    a[1]=initValues[1];
    a[2]=initValues[2];
    a[3]=initValues[3];
    a[4]=initValues[4];
    a[5]=initValues[5];
    a[6]=initValues[6];
    a[7]=initValues[7];
    a[8]=initValues[8];
    a[9]=initValues[9];
    a[10]=initValues[10];
    a[11]=initValues[11];
    a[12]=initValues[12];
    a[13]=initValues[13];
    a[14]=initValues[14];
    a[15]=initValues[15];
}

Matrix4D::Matrix4D(const float initValues[4][4]){
    a4x4[0][0]=initValues[0][0];
    a4x4[0][1]=initValues[0][1];
    a4x4[0][2]=initValues[0][2];
    a4x4[0][3]=initValues[0][3];

    a4x4[1][0]=initValues[1][0];
    a4x4[1][1]=initValues[1][1];
    a4x4[1][2]=initValues[1][2];
    a4x4[1][3]=initValues[1][3];

    a4x4[2][0]=initValues[2][0];
    a4x4[2][1]=initValues[2][1];
    a4x4[2][2]=initValues[2][2];
    a4x4[2][3]=initValues[2][3];

    a4x4[3][0]=initValues[3][0];
    a4x4[3][1]=initValues[3][1];
    a4x4[3][2]=initValues[3][2];
    a4x4[3][3]=initValues[3][3];
}

Matrix4D::Matrix4D(
        float _a11,float _a12,float _a13,float _a14,
        float _a21,float _a22,float _a23,float _a24,
        float _a31,float _a32,float _a33,float _a34,
        float _a41,float _a42,float _a43,float _a44
        ){
    a11=_a11;
    a12=_a12;
    a13=_a13;
    a14=_a14;

    a21=_a21;
    a22=_a22;
    a23=_a23;
    a24=_a24;

    a31=_a31;
    a32=_a32;
    a33=_a33;
    a34=_a34;

    a41=_a41;
    a42=_a42;
    a43=_a43;
    a44=_a44;
}

RawImage::RawImage(int _w,int _h){
    pix=new ColorRGBA[_w*_h];
    w=_w;
    h=_h;
}
RawImage::~RawImage(){
    SAFE_DELETE_A(pix);
}

Material::Material(){
    brdf=NULL;
    emission=0;
    refraction=0;
    reflection=0;
    ior=1.0f;
    emitColor.r=1.0f;
    emitColor.g=1.0f;
    emitColor.b=1.0f;
    emitPower=0.0f;
}

Material::~Material(){
    SAFE_DELETE_O(brdf);
}
