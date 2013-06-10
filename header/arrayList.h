/**
 * @file arrayList.h
 *
 * @author take
 */

#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

template<class T>class ArrayList{
private:
    int size;
    int num;
    T* array;
    void expand(int nsize);
public:
    ArrayList(int size);
    ~ArrayList();
    int getSize();
    int getArraySize();
    int add(T o);
    int insert(T o,int index);
    T remove(int i);
    T remove(T o);
    T get(int i);
    void clearAll();
};

//------------------------------------------ArrayListŽÀ‘•---------------------

template<class T>ArrayList<T>::ArrayList(int size){
    this->size=size;
    num=0;
    array=new T[size];
    for(int i=0;i<size;i++){
        array[i]=NULL;
    }
}
template<class T>ArrayList<T>::~ArrayList(){
    delete[] array;
}
template<class T>void ArrayList<T>::expand(int nsize){
    T* narray=new T[nsize];
    int i=0;
    for(i=0;i<size;i++){
        narray[i]=array[i];
    }
    for(;i<nsize;i++){
        narray[i]=NULL;
    }
    size=nsize;
    delete[] array;
    array=narray;
}
template<class T>int ArrayList<T>::getSize(){
    return num;
}
template<class T>int ArrayList<T>::getArraySize(){
    return size;
}
template<class T>int ArrayList<T>::add(T o){
    if(num>=size){
        expand(size+size/2);
    }
    array[num]=o;
    num++;
    return num;
}
template<class T>int ArrayList<T>::insert(T o,int index){
    for(int i=num;i>index;i--){
        array[i]=array[i-1];
    }
    array[index]=o;
    num++;
    return num;
}
template<class T>T ArrayList<T>::remove(int i){
    T ret=array[i];
    num--;
    for(int j=i;j<num;j++){
        array[j]=array[j+1];
    }
    return ret;
}
template<class T>T ArrayList<T>::remove(T o){
    for(int i=0;i<num;i++){
        if(o==array[i]){
            return remove(i);
        }
    }
    return NULL;
}
template<class T>T ArrayList<T>::get(int i){
    return array[i];
}
template<class T>void ArrayList<T>::clearAll(){
    num=0;
}


#endif /* ARRAYLIST_H_ */
