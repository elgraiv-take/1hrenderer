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
    virtual void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret)=0;
};

class Lambert:public BRDF{
private:
    ColorRGBA color;
public:
    void setColor(ColorRGBA& color);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class TexturedLambert:public BRDF{
private:
//    ColorRGBA color;
    RawImage* tex;
public:
    void setTexture(RawImage* img);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class BRDFTable:public BRDF{
private:
    RawImage* table;
public:
    void setTexture(RawImage* _table);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class BinormalBRDFTable:public BRDF{
private:
    RawImage* table;
public:
    void setTexture(RawImage* _table);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class BinormalBRDFTableForHair:public BRDF{
private:
    RawImage* table;
public:
    void setTexture(RawImage* _table);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

class Phong:public BRDF{
private:
    Lambert deff;
    ColorRGBA sColor;
    float specPow;
    float specHard;
public:
    Phong(){
        specPow=0.4;
        specHard=10.0f;
    }
    void setSpecParam(float _specPow,float _specHard);
    void setDeffColor(ColorRGBA& color);
    void setSpecColor(ColorRGBA& color);
    void function(RayIntersection& ri,Vector4D& normal,Vector4D& in,Vector4D& out,Vector4D& tangent,ColorRGBA& ret);
};

#endif /* BRDF_H_ */
