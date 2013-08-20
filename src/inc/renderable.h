/*
 * renderable.h
 *
 *  Created on: 2013/08/09
 *      Author: take
 */

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "root.h"
#include "structDef.h"

class Renderable{
public:
    virtual ~Renderable(){}
    virtual int getIntersection(const RayObject& ray,RayIntersection& intersection)=0;
    virtual void getPoint(const RayIntersection& intersection,Vector4D& p)=0;
    virtual void getUV(const RayIntersection& intersection,VectorUV& uv)=0;
    virtual int getNormal(const RayIntersection& intersection,Vector4D& normal)=0;
    virtual void getTangent(const RayIntersection& intersection,Vector4D& tangent)=0;
    virtual Material* getMaterial()=0;
};


#endif /* RENDERABLE_H_ */
