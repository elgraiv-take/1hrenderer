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

//    public void setPixel(VertexNVLTB v, int bufindex) {
//
//
//        float itheta=getTheta(v.vNormal,v.vTangent,v.vLight);
//        float otheta=(getTheta(v.vNormal,v.vTangent,v.vView)+90)%360;
//        itheta=Math.min(Math.max(itheta,0.0f),180.0f);
//
//        int idx=((int)itheta)+((int)otheta)*INPUT_ANGLES;
//
//
//        float refA=getAmount(v.vNormal,v.vBinormal,v.vLight,v.vView);
//        float r = this.brdfTable[idx].r*light.color.r*refA;
//        float g = this.brdfTable[idx].g*light.color.g*refA;
//        float b = this.brdfTable[idx].b*light.color.b*refA;
//
//    }
//
//    private static float getAmount(Vector3 n,Vector3 bn,Vector3 l,Vector3 v){
//        float vx=Vector3.dotProduct(bn,v);
//        float vy=Vector3.dotProduct(n,v);
//        float lx=-Vector3.dotProduct(bn,l);
//        float ly=Vector3.dotProduct(n,l);
//        float ret=vx*lx+vy*ly;
//        ret=(ret+1.0f)/2.0f;
//        ret=Math.max(Math.min(ret,1.0f),0.0f);
//        ret=(float)Math.pow(ret,2.0f);
//        return ret;
//    }
//
//    private static float getTheta(Vector3 n,Vector3 t,Vector3 v){
//        float x=Vector3.dotProduct(t,v);
//        float y=Vector3.dotProduct(n,v);
//        float norm=(float)Math.sqrt(x*x+y*y);
//        x/=norm;
//        y/=norm;
//        return (float)Math.toDegrees(Math.acos(x));
//    }
