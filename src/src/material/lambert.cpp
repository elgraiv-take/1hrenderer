/**
 * @file lambert.cpp
 *
 * @author take
 */


#include "brdf.h"
#include "vectorMath.h"

void Lambert::setColor(ColorRGBA& color){
    this->color=color;
}

void Lambert::function(Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){
    float dotp=VectorMath::dot(normal,in);
    if(dotp<0.0f){
        dotp=0.0f;
    }
    ret.r=dotp*color.r;
    ret.g=dotp*color.g;
    ret.b=dotp*color.b;

}


