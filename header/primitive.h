/**
 * @file primitive.h
 *
 * @author take
 */

#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

#include "dataStructure.h"

namespace PrimitiveGen {

PolygonMesh* createPlane(float w,float h);
PolygonMesh* createSphere(float size,int divh,int divv);

}  // namespace PrimitiveGen


#endif /* PRIMITIVE_H_ */
