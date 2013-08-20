/**
 * @file brdf.h
 *
 * @author take
 */

#ifndef BRDF_H_
#define BRDF_H_

#include "structDef.h"

class BRDF{
public:
    virtual ~BRDF(){}
    virtual void function(Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret)=0;
};

class Lambert:public BRDF{
private:
    ColorRGBA color;
public:
    void setColor(ColorRGBA& color);
    void function(Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class Phong:public BRDF{
private:
    Lambert deff;
    ColorRGBA sColor;
    float specPow;
    float specHard;
public:
    void setDeffColor(ColorRGBA& color);
    void setSpecColor(ColorRGBA& color);
    void function(Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

#endif /* BRDF_H_ */
