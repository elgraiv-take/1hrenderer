/**
 * @file largeArray.h
 *
 * @author take
 */

#ifndef LARGEARRAY_H_
#define LARGEARRAY_H_

#include "root.h"

#define MAX_SIZE_PAR_ARRAY (10000000)

template<class T> class LargeArray{
private:
    T** elem;
    int arraySize;
    int arrayNum;
    int lastSize;
    int size;
    public:
    LargeArray(int n){
        size=n;
        arraySize=MAX_SIZE_PAR_ARRAY/sizeof(T);
        arrayNum=(size/arraySize)+1;
        lastSize=size-(arrayNum-1)*arraySize;
        elem=new T*[arrayNum];
        for(int i=0;i<arrayNum-1;i++){
            elem[i]=new T[arraySize];
        }
        elem[arrayNum-1]=new T[lastSize];
    }
    ~LargeArray(){
        for(int i=0;i<arrayNum;i++){
            delete[] elem[i];
        }
        delete[] elem;
    }
    void set(int i,T& o){
        int aind=i/arraySize;
        int eind=i%arraySize;
        elem[aind][eind]=o;
    }

    T get(int i){
        int aind=i/arraySize;
        int eind=i%arraySize;
        return elem[aind][eind];
    }
    T* getp(int i){
        int aind=i/arraySize;
        int eind=i%arraySize;
        return &(elem[aind][eind]);
    }
    int length(){
        return size;
    }
};

#endif /* LARGEARRAY_H_ */
