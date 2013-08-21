/*
 * brdftable.cpp
 *
 *  Created on: 2013/08/19
 *      Author: take
 */

#include "brdf.h"
#include "vectorMath.h"

#define INPUT_ANGLES (181)
#define OUTPUT_ANGLES (361)

namespace BinormalBRDFTableUtil {
float getTheta(Vector4D n,Vector4D t,Vector4D v){
    float x=VectorMath::dot(t,v);
    float y=VectorMath::dot(n,v);
    float norm=sqrt(x*x+y*y);
    x/=norm;
    y/=norm;
    return TO_DEG(acos(x));
}

float getAmount(Vector4D n,Vector4D bn,Vector4D l,Vector4D v){
    float vx=VectorMath::dot(bn,v);
    float vy=VectorMath::dot(n,v);
    float lx=-VectorMath::dot(bn,l);
    float ly=VectorMath::dot(n,l);
    float ret=vx*lx+vy*ly;
    ret=(ret+1.0f)/2.0f;
    ret=MAX(MIN(ret,1.0f),0.0f);
    ret=pow(ret,2.0f);
    return ret;
}

} // namespace BinormalBRDFTableUtil

void BinormalBRDFTable::setTexture(RawImage* _table){
    table=_table;
}
void BinormalBRDFTable::function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){
    Vector4D binormal,modtan;
    VectorMath::cross(normal,tangent,binormal);
    VectorMath::normalize(binormal);
    VectorMath::cross(binormal,normal,modtan);
    VectorMath::normalize(modtan);
//    float itheta=BinormalBRDFTableUtil::getTheta(normal,binormal,out);
//    int otheta=((int)(BinormalBRDFTableUtil::getTheta(normal,binormal,in)+90))%360;
    float itheta=BinormalBRDFTableUtil::getTheta(normal,modtan,out);
    int otheta=((int)(BinormalBRDFTableUtil::getTheta(normal,modtan,in)+90))%360;
    itheta=MIN(MAX(itheta,0.0f),180.0f);

    int idx=((int)itheta)+otheta*INPUT_ANGLES;
    int size=INPUT_ANGLES*OUTPUT_ANGLES-1;
    idx=MIN(MAX(idx,0),size);

//    float refA=BinormalBRDFTableUtil::getAmount(normal,modtan,out,in);
    float refA=BinormalBRDFTableUtil::getAmount(normal,binormal,out,in);
    ret.r=table->pix[idx].r*refA;
    ret.g=table->pix[idx].g*refA;
    ret.b=table->pix[idx].b*refA;
}

void BinormalBRDFTableForHair::setTexture(RawImage* _table){
    table=_table;
}
void BinormalBRDFTableForHair::function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){
    Vector4D binormal,modtan;
    VectorMath::cross(normal,tangent,binormal);
    VectorMath::normalize(binormal);
    VectorMath::cross(binormal,normal,modtan);
    VectorMath::normalize(modtan);
    float itheta=BinormalBRDFTableUtil::getTheta(normal,binormal,out);
    int otheta=((int)(BinormalBRDFTableUtil::getTheta(normal,binormal,in)+90))%360;
//    float itheta=BinormalBRDFTableUtil::getTheta(normal,modtan,out);
//    int otheta=((int)(BinormalBRDFTableUtil::getTheta(normal,modtan,in)+90))%360;
    itheta=MIN(MAX(itheta,0.0f),180.0f);

    int idx=((int)itheta)+otheta*INPUT_ANGLES;
    int size=INPUT_ANGLES*OUTPUT_ANGLES-1;
    idx=MIN(MAX(idx,0),size);

    float refA=BinormalBRDFTableUtil::getAmount(normal,modtan,out,in);
//    float refA=BinormalBRDFTableUtil::getAmount(normal,binormal,out,in);
    ret.r=table->pix[idx].r*refA;
    ret.g=table->pix[idx].g*refA;
    ret.b=table->pix[idx].b*refA;
}
