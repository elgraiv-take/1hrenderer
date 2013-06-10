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
void Phong::function(Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){
    specPow=0.4f;
    specHard=4.0f;
    ColorRGBA deffc;
    deff.function(normal,in,out,tangent,deffc);
    Vector4D refv;
    VectorMath::reflectionVector(in,normal,refv);
    float power=VectorMath::dot(refv,out);
    float spec=pow(power,specHard);
    float deffP=(1.0f-specPow);
    ret.r=deffc.r*deffP+sColor.r*specPow;
    ret.g=deffc.g*deffP+sColor.g*specPow;
    ret.b=deffc.b*deffP+sColor.b*specPow;
}
