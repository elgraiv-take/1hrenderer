/**
 * @file xorshift.h
 *
 * @author take
 */

#ifndef XOSHIFT_H_
#define XOSHIFT_H_

#include <stdlib.h>

class Xorshift{
private:
    static unsigned int x;
    static unsigned int y;
    static unsigned int z;
    static unsigned int w;

public:
    static inline unsigned int next(){
        unsigned int t=x^(x<<11);
        x=y;
        y=z;
        z=w;
        return w=(w^(w>>19))^(t^(t>>8));
    }

    static inline float nextf(){
//        int r=rand();
//        return r/(float)RAND_MAX;
        unsigned int i=next();
        return i/4294967296.0;
    }

    static void setSeed(
            unsigned int x,
            unsigned int y,
            unsigned int z,
            unsigned int w
            ){
        Xorshift::x=x;
        Xorshift::y=y;
        Xorshift::z=z;
        Xorshift::w=w;
    }
};

#endif /* XOSHIFT_H_ */
