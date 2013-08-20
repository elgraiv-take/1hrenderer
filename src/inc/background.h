/**
 * @file background.h
 *
 * @author take
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "dataStructure.h"

class SphereImageBG:public IBackGround{
    RawImage* bgi;
    float pow;
public:
    SphereImageBG();
    ~SphereImageBG();
    void setImage(RawImage* _bgi);
    void setPower(float _pow);
    void getBGColor(Vector4D& vec,ColorRGBA& bgc);
};

class ConstColorBG:public IBackGround{
    ColorRGBA color;
    float pow;
public:
    void setColor(ColorRGBA& _color);
    void setPower(float _pow);
    void getBGColor(Vector4D& vec,ColorRGBA& bgc);
};

#endif /* BACKGROUND_H_ */
