/*
 * texturemap.cpp
 *
 *  Created on: 2013/08/19
 *      Author: Take
 */

#include "brdf.h"
#include "renderable.h"
#include "vectorMath.h"

void TexturedLambert::setTexture(RawImage* img){
    tex=img;
}
void TexturedLambert::function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret){
    VectorUV uv;
    ri.id->getUV(ri,uv);
    int iu=(int)(uv.u*tex->w);
    int iv=(int)(uv.v*tex->h);
    iu=MAX(MIN(iu,(tex->w-1)),0);
    iv=MAX(MIN(iv,(tex->h-1)),0);
    float dotp=VectorMath::dot(normal,in);
    if(dotp<0.0f){
        dotp=0.0f;
    }
    iv=(tex->h-iv-1);
    ret.r=tex->pix[iv*tex->w+iu].r*dotp;
    ret.g=tex->pix[iv*tex->w+iu].g*dotp;
    ret.b=tex->pix[iv*tex->w+iu].b*dotp;
}
