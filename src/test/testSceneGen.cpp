/**
 * @file testSceneGen.cpp
 *
 * @author take
 */

#include "test.h"
#include "vectorMath.h"
#include "camera.h"
#include "background.h"
#include "brdf.h"
#include "meshloader.h"
#include "imageIO.h"

IFCamera* testCamera(){
    PersCamera* ret=new PersCamera();
    Vector4D pos(7.7,-7.2,3.5);
    ret->setPosition(pos);
    Vector4D rot(TO_RAD(70.0),0.0,TO_RAD(45));
    ret->setRotation(rot);
    ret->setAOV(TO_RAD(49.134));
    ret->setDepth(0.0f,100.0f);
    return ret;
}

IFCamera* testCamera2(){
    PersCamera* ret=new PersCamera();
    Vector4D pos(0.0,-14,5);
    ret->setPosition(pos);
    Vector4D rot(TO_RAD(90.0),0.0,TO_RAD(0.0));
    ret->setRotation(rot);
//    ret->setAOV(TO_RAD(49.134));
    ret->setAOV(TO_RAD(55));
    ret->setDepth(0.0f,100.0f);
    return ret;
}

Scene* testScene2(){
    Scene* ret=new Scene();
    ret->setCamera(testCamera2());

    Vector4D trans;
    Vector4D rot;

    Material* white=new Material();
    Lambert* wl=new Lambert();
    ColorRGBA color;
    color.a=color.r=color.g=color.b=1.0f;
    wl->setColor(color);
    white->brdf=wl;
    ret->addMaterial(white);

    Material* greenP=new Material();
    Phong* gp=new Phong();
    color.a=color.r=color.g=color.b=1.0f;
    gp->setSpecColor(color);
    color.r=color.b=0.0f;
    gp->setDeffColor(color);
    greenP->brdf=gp;
    ret->addMaterial(greenP);
    color.a=color.r=color.g=color.b=1.0f;

    PolygonMesh* plane=PrimitiveGen::createPlane(10,16);
    plane->setMaterialToAll(white);
    ret->addMesh(plane);

    plane=PrimitiveGen::createPlane(10,16);
    plane->setMaterialToAll(white);
    rot.x=TO_RAD(180.0f);
    trans.z=10.0f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    plane=PrimitiveGen::createPlane(4,4);
    Material* light=new Material();
    light->emission=1;
    light->emitPower=20.0;
    ret->addMaterial(light);
    plane->setMaterialToAll(light);
    trans.z=9.9f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    plane=PrimitiveGen::createPlane(10,10);
    plane->setMaterialToAll(white);
    rot.x=TO_RAD(90.0f);
    trans.z=5.0f;
    trans.y=8.0f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    Material* red=new Material();
    Lambert* rl=new Lambert();
    color.g=color.b=0.0f;
    rl->setColor(color);
    red->brdf=rl;
    ret->addMaterial(red);

    plane=PrimitiveGen::createPlane(10,16);
    plane->setMaterialToAll(red);
    rot.y=TO_RAD(90.0f);
    rot.x=0.0f;
    trans.x=-5.0f;
    trans.y=0.0f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    Material* blue=new Material();
    Lambert* bl=new Lambert();
    color.b=1.0f;
    color.r=0.0f;
    bl->setColor(color);
    blue->brdf=bl;
    ret->addMaterial(blue);

    plane=PrimitiveGen::createPlane(10,16);
    plane->setMaterialToAll(blue);
    rot.y=TO_RAD(-90.0f);
    trans.x=5.0f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    Material* mirror=new Material();
    mirror->reflection=1;
    ret->addMaterial(mirror);

    PolygonMesh* sphere=PrimitiveGen::createSphere(4,12,24);
//    PolygonMesh* sphere=PrimitiveGen::createSphere(4,8,16);
//    sphere->setMaterialToAll(mirror);
    sphere->setMaterialToAll(greenP);
    trans.x=-2.0;
    trans.y=4;
    trans.z=2;
    sphere->translate(trans);
    ret->addMesh(sphere);

    Material* glass=new Material();
    glass->refraction=1;
    glass->ior=1.51;
    ret->addMaterial(glass);

    sphere=PrimitiveGen::createSphere(3,12,24);
//    sphere=PrimitiveGen::createSphere(3,8,16);
    sphere->setMaterialToAll(glass);
    trans.x=2.5;
    trans.y=2.0;
    trans.z=1.5;
    sphere->translate(trans);
    ret->addMesh(sphere);

    return ret;
}

//IFCamera* testCamera2(){
//    PersCamera* ret=new PersCamera();
//    Vector4D pos(0,0,10);
//    ret->setPosition(pos);
//    Vector4D rot(TO_RAD(70.0),0.0,TO_RAD(45));
//    ret->setRotation(rot);
//    ret->setAOV(TO_RAD(49.134));
////    ret->setDepth(0.0f,100.0f);
//    return ret;
//}

Scene* testScene4(){
    Scene* ret=new Scene();

    Vector4D trans;

    Material* white=new Material();
    Lambert* wl=new Lambert();
    ColorRGBA color;
    color.a=color.r=color.g=color.b=1.0f;
    wl->setColor(color);
    white->brdf=wl;
    ret->addMaterial(white);

    Material* mirror=new Material();
    mirror->reflection=1;
    ret->addMaterial(mirror);

    Material* glass=new Material();
    glass->refraction=1;
    glass->ior=1.51;
    ret->addMaterial(glass);

    PolygonMesh* plane=PrimitiveGen::createPlane(10,10);
    plane->setMaterialToAll(white);
    ret->addMesh(plane);

    //----------------------------------------------------------------

//    PolygonMesh* sphere1=PrimitiveGen::createSphere(3.0,16,32);
//    sphere1->setMaterialToAll(white);

//    sphere1->translate(trans);
    PolygonMesh* bunny=MeshLoader::readFile("bunny.mesh");
    bunny->setMaterialToAll(glass);
    bunny->scale(20.0f);
    Vector4D rot(TO_RAD(90.0f),0.0f,TO_RAD(30.0f));
    bunny->rotate(rot);
    trans.x=3.0;
    trans.y=-2.0;
    trans.z=-0.5;
    bunny->translate(trans);
    ret->addMesh(bunny);
    ret->setCamera(testCamera());

    RawImage* bgi;
//    ImageIO::readHDR("bg.hdr",&bgi);
    ImageIO::readHDR("gr.hdr",&bgi);
    SphereImageBG* bg=new SphereImageBG();
    bg->setImage(bgi);

    bg->setPower(1.0f);
    ret->setBackGround(bg);

    return ret;
}

Scene* testScene3(){
    Scene* ret=new Scene();

    Vector4D trans;

    Material* white=new Material();
    Lambert* wl=new Lambert();
    ColorRGBA color;
    color.a=color.r=color.g=color.b=1.0f;
    wl->setColor(color);
    white->brdf=wl;
    ret->addMaterial(white);

    Material* mirror=new Material();
    mirror->reflection=1;
    ret->addMaterial(mirror);

    Material* glass=new Material();
    glass->refraction=1;
    glass->ior=1.51;
    ret->addMaterial(glass);

    PolygonMesh* plane=PrimitiveGen::createPlane(10,10);
    plane->setMaterialToAll(white);
    ret->addMesh(plane);

    //----------------------------------------------------------------

    PolygonMesh* sphere1=PrimitiveGen::createSphere(3.0,16,32);
    sphere1->setMaterialToAll(white);
    trans.x=-1.46;
    trans.y=0.178;
    trans.z=1.5;
    sphere1->translate(trans);
    PolygonMesh* sphere2=PrimitiveGen::createSphere(2.0,16,32);
    sphere2->setMaterialToAll(mirror);
    trans.x=1.77;
    trans.y=0.198;
    trans.z=1.0;
    sphere2->translate(trans);
    PolygonMesh* sphere3=PrimitiveGen::createSphere(1.5,16,32);
    sphere3->setMaterialToAll(glass);
    trans.x=2.4;
    trans.y=-2.22;
    trans.z=0.75;
    sphere3->translate(trans);
    ret->addMesh(sphere1);
    ret->addMesh(sphere2);
    ret->addMesh(sphere3);
    //------------------------------------------------------------
//    PolygonMesh* sphere1=PrimitiveGen::createSphere(3.0,16,32);
//    PolygonMesh* sphere1=PrimitiveGen::createSphere(3.0,8,16);
//    sphere1->setMaterialToAll(white);
//    sphere1->setMaterialToAll(mirror);
//    trans.x=1.77;
//    trans.y=0.198;
//    trans.z=1.5;
//    sphere1->translate(trans);
//    ret->addMesh(sphere1);

//------------------------------------------------------------
    ret->setCamera(testCamera());

//    ConstColorBG* bg=new ConstColorBG();
//    color.r=0.5;
//    color.g=0.8;
//    bg->setColor(color);

    RawImage* bgi;
//    ImageIO::readHDR("D:/workspace/eclipseWS2013/girender/data/bg.hdr",&bgi);
//    ImageIO::readHDR("epc.hdr",&bgi);
//    ImageIO::readHDR("MonValley_A_LookoutPoint_2k.hdr",&bgi);
    ImageIO::readHDR("PaperMill_E_3k.hdr",&bgi);

    SphereImageBG* bg=new SphereImageBG();
    bg->setImage(bgi);

//    bg->setPower(1.0f);
    bg->setPower(0.1f);
    ret->setBackGround(bg);

    return ret;
}

Scene* testScene1(){
    Scene* ret=new Scene();
    PolygonMesh* plane=PrimitiveGen::createPlane(10,10);
    ret->addMesh(plane);
    Vector4D trans;
    PolygonMesh* sphere1=PrimitiveGen::createSphere(3.0,16,32);
    trans.x=-1.46;
    trans.y=0.178;
    trans.z=1.5;
    sphere1->translate(trans);
    PolygonMesh* sphere2=PrimitiveGen::createSphere(2.0,16,32);
    trans.x=1.77;
    trans.y=0.198;
    trans.z=1.0;
    sphere2->translate(trans);
    PolygonMesh* sphere3=PrimitiveGen::createSphere(1.5,16,32);
    trans.x=2.4;
    trans.y=-2.22;
    trans.z=0.75;
    sphere3->translate(trans);
    ret->addMesh(sphere1);
    ret->addMesh(sphere2);
    ret->addMesh(sphere3);
    ret->setCamera(testCamera());
    return ret;
}

Scene* getTestScene(){
    return testScene2();
}

