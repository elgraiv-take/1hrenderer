/**
 * @file phong.cpp
 *
 * @author take
 */

#include "brdf.h"
#include "vectorMath.h"

void Phong::setDeffColor(ColorRGBA& color){
    deff.setColor(color);
}
void Phong::setSpecColor(ColorRGBA& color){
    sColor=color;
}
void Phong::setSpecParam(float _specPow,float _specHard){
    specPow=_specPow;
    specHard=_specHard;
}

void Phong::function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){

    ColorRGBA deffc;
    deff.function(ri,normal,in,out,tangent,deffc);
    Vector4D refv;
    VectorMath::reflectionVector(in,normal,refv);
    float power=VectorMath::dot(refv,out);
    power=MIN(MAX(power,0.0f),1.0f);
    float spec=pow(power,specHard);
//    printf("%f\n",spec);
    float deffP=(1.0f-specPow);
//    printf("%f %f\n",specPow,spec,sColor.r*specPow);
    ret.r=deffc.r*deffP+sColor.r*specPow*spec;
    ret.g=deffc.g*deffP+sColor.g*specPow*spec;
    ret.b=deffc.b*deffP+sColor.b*specPow*spec;
}
