/**
 * @file scene.cpp
 *
 * @author take
 */

#include "dataStructure.h"

Scene::Scene(){
    meshes=new ArrayList<PolygonMesh*>(10);
    lights=new ArrayList<IFLight*>(5);
    materials=new ArrayList<Material*>(5);
    ishairs=new ArrayList<IsHair*>(5);
    camera=NULL;
    bg=NULL;
}
Scene::~Scene(){
    int n=meshes->getSize();
    for(int i=0;i<n;i++){
        PolygonMesh* tm=meshes->get(i);
        SAFE_DELETE_O(tm);
    }
    n=lights->getSize();
    for(int i=0;i<n;i++){
        IFLight* tl=lights->get(i);
        SAFE_DELETE_O(tl);
    }
    n=materials->getSize();
    for(int i=0;i<n;i++){
        Material* mt=materials->get(i);
        SAFE_DELETE_O(mt);
    }
    SAFE_DELETE_O(meshes);
    SAFE_DELETE_O(lights);
    SAFE_DELETE_O(materials);
    SAFE_DELETE_O(ishairs);
    SAFE_DELETE_O(camera);
}
void Scene::addMesh(PolygonMesh* m){
    meshes->add(m);
}
void Scene::addLight(IFLight* l){
    lights->add(l);
}
void Scene::addMaterial(Material* m){
    materials->add(m);
}
void Scene::addIsHair(IsHair* h){
    ishairs->add(h);
}
void Scene::setCamera(IFCamera* c){
    SAFE_DELETE_O(camera);
    camera=c;
}
void Scene::setBackGround(IBackGround* _bg){
    SAFE_DELETE_O(bg);
    bg=_bg;
}

ArrayList<PolygonMesh*>* Scene::getMesheList(){
    return meshes;
}
ArrayList<IsHair*>* Scene::getIsHairList(){
    return ishairs;
}
ArrayList<IFLight*>* Scene::getLightList(){
    return lights;
}
IFCamera* Scene::getCamera(){
    return camera;
}

IBackGround* Scene::getBackGround(){
    return bg;
}
