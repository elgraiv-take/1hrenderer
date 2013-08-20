/**
 * @file light.h
 *
 * @author take
 */

#ifndef LIGHT_H_
#define LIGHT_H_

#include "dataStructure.h"

class PointLight:public IFLight{
private:
    float power;
    Vector4D co;
public:
    void getDirection(Vector4D& pos,Vector4D& direction);
    float getPower(Vector4D& pos);
};



#endif /* LIGHT_H_ */
