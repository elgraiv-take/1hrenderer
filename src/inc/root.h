/**
 * @file root.h
 *
 * @author take
 */

#ifndef ROOT_H_
#define ROOT_H_

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#ifndef NULL
#define NULL 0
#endif
#define SAFE_DELETE_A(a) if(a){delete[] a;a=0;}
#define SAFE_DELETE_O(o) if(o){delete o;o=0;}
#define TO_RAD(d) ((M_PI*d)/180.0f)
#define TO_DEG(r) ((r*180.0f)/M_PI)
#define FLOAT_MIN_VAL (0.0000001f)
#define FLOAT_MAX_VAL (100000000.0f)
#define MAX(a1,a2) ((a1)>(a2)?(a1):(a2))
#define MIN(a1,a2) ((a1)<(a2)?(a1):(a2))

class BRDF;

#endif /* ROOT_H_ */
