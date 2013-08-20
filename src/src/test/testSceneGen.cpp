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

    Material* texture=new Material();
    TexturedLambert* tl=new TexturedLambert();
    RawImage* teximg;
    ImageIO::readRawData("test.tex",&teximg);
    tl->setTexture(teximg);
    texture->brdf=tl;
    ret->addMaterial(texture);

    Material* morpho=new Material();
    BinormalBRDFTable* brdft=new BinormalBRDFTable();
    RawImage* table;
    ImageIO::readBRDF("brdf.txt",&table);
    brdft->setTexture(table);
    morpho->brdf=brdft;
    ret->addMaterial(morpho);

    IsHair* hair=IsHairLoader::createTestIsHairObject();
    ret->addIsHair(hair);
//    hair->setMaterial(white);
    hair->setMaterial(morpho);

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

    plane=PrimitiveGen::createPlane(1,1);
    Material* light=new Material();
    light->emission=1;
    light->emitPower=20.0;
    light->emitPower=400.0;
    ret->addMaterial(light);
    plane->setMaterialToAll(light);
    trans.z=9.9f;
    plane->rotate(rot);
    plane->translate(trans);
    ret->addMesh(plane);

    plane=PrimitiveGen::createPlane(10,10);
//    plane->setMaterialToAll(white);
    plane->setMaterialToAll(texture);
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
    sphere->setMaterialToAll(mirror);
//    sphere->setMaterialToAll(greenP);
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

////////-------------------------------------------//TODO Scene

IFCamera* render1hCamera(){
    PersCamera* ret=new PersCamera();
    Vector4D pos(5.7,-12.4,16.4);
    ret->setPosition(pos);
//    Vector4D rot(TO_RAD(81.0),0.0,TO_RAD(14));
    Vector4D rot(TO_RAD(76.0),0.0,TO_RAD(19));
    ret->setRotation(rot);
//    ret->setAOV(TO_RAD(67.38));
    ret->setAOV(TO_RAD(49.134));
    ret->setDepth(0.0f,100.0f);
    return ret;
}

Scene* render1hScene(){
    Scene* ret=new Scene();

    Vector4D loc,rot;
    ColorRGBA color;
    ///----------------Light
    //material
    Material* roomlight=new Material();
    roomlight->emission=1;
    roomlight->emitPower=20000.0;
    roomlight->emitColor.r=1.0f;
    roomlight->emitColor.g=0.8f;
    roomlight->emitColor.b=0.6f;
    ret->addMaterial(roomlight);
    //mesh
    PolygonMesh* lightMesh=MeshLoader::readFile("light.msh");
    lightMesh->setMaterialToAll(roomlight);
    ret->addMesh(lightMesh);

    ///----------------Light
    //material
    Material* tvlight=new Material();
    tvlight->emission=1;
    tvlight->emitPower=15000.0;
    tvlight->emitColor.r=0.7f;
    tvlight->emitColor.g=0.9f;
    tvlight->emitColor.b=1.0f;
    ret->addMaterial(tvlight);
    //mesh
    PolygonMesh* tvMesh=MeshLoader::readFile("tv.msh");
    tvMesh->setMaterialToAll(tvlight);
    loc.x=-13.0f;
    loc.y=-5.4f;
    loc.z=13.0f;
    tvMesh->translate(loc);
    rot.x=TO_RAD(90.0f);
    rot.x=TO_RAD(0.0f);
    rot.x=TO_RAD(120.0f);
    tvMesh->rotate(rot);
    ret->addMesh(tvMesh);

    ///----------------Karaoke Room
    //material
    Material* roomwall=new Material();
    TexturedLambert* rwtl=new TexturedLambert();
    RawImage* wallpaper;
    ImageIO::readRawData("wall.tex",&wallpaper);
    rwtl->setTexture(wallpaper);
    roomwall->brdf=rwtl;
    ret->addMaterial(roomwall);
    //mesh
    PolygonMesh* roomMesh=MeshLoader::readFile("room.msh");
//    PolygonMesh* roomMesh=PrimitiveGen::createPlane(36.0f,27.0f);
    roomMesh->setMaterialToAll(roomwall);
    ret->addMesh(roomMesh);

    ////------------------Sofa
    //material
    Material* sofaMat=new Material();
    Lambert* redl=new Lambert();
    color.r=0.9f;
    color.g=0.05f;
    color.b=0.0f;
    redl->setColor(color);
    sofaMat->brdf=redl;
    ret->addMaterial(sofaMat);
    //mesh
    PolygonMesh* sofa1=MeshLoader::readFile("sofa.msh");
    sofa1->setMaterialToAll(sofaMat);
    loc.x=-12.5f;
    loc.y=9.9f;
    loc.z=0.0f;
    sofa1->translate(loc);
    ret->addMesh(sofa1);
    //mesh2
    PolygonMesh* sofa2=MeshLoader::readFile("sofa.msh");
    sofa2->setMaterialToAll(sofaMat);
    loc.x=-1.65f;
    loc.y=9.9f;
    loc.z=0.0f;
    sofa2->translate(loc);
    ret->addMesh(sofa2);
    //mesh3
    PolygonMesh* sofa3=MeshLoader::readFile("sofa.msh");
    sofa3->setMaterialToAll(sofaMat);
    loc.x=9.2f;
    loc.y=9.9f;
    loc.z=0.0f;
    sofa3->translate(loc);
    ret->addMesh(sofa3);

    ////-----------------------Table
    //material
    Material* tableMat=new Material();
    Phong* wphong=new Phong();
    color.r=1.0f;
    color.g=1.0f;
    color.b=1.0f;
    wphong->setDeffColor(color);
    wphong->setSpecColor(color);
    tableMat->brdf=wphong;
    ret->addMaterial(tableMat);
    //mesh1
    PolygonMesh* table1=MeshLoader::readFile("table.msh");
    table1->setMaterialToAll(tableMat);
    loc.x=-6.0f;
    loc.y=0.0f;
    loc.z=0.0f;
    table1->translate(loc);
    ret->addMesh(table1);
    PolygonMesh* table2=MeshLoader::readFile("table.msh");
    table2->setMaterialToAll(tableMat);
    loc.x=5.6f;
    loc.y=-0.1f;
    loc.z=0.0f;
    table2->translate(loc);
    rot.x=0.0f;
    rot.y=0.0f;
    rot.z=TO_RAD(3.0f);
    table2->rotate(rot);
    ret->addMesh(table2);

    ////----------------------Glass
    //material
    Material* glass=new Material();
    glass->refraction=1;
    glass->ior=1.51;
    ret->addMaterial(glass);
    //glass mesh
    PolygonMesh* glassMesh=MeshLoader::readFile("glass.msh");
    glassMesh->setMaterialToAll(glass);
    loc.x=-2.7f;
    loc.y=2.5f;
    loc.z=5.577f;
    glassMesh->translate(loc);
    ret->addMesh(glassMesh);

    ///----------------Body
    //material
    Material* bodymat=new Material();
    TexturedLambert* bodytl=new TexturedLambert();
    RawImage* bodytex;
    ImageIO::readRawData("body.tex",&bodytex);
    bodytl->setTexture(bodytex);
    bodymat->brdf=bodytl;
    ret->addMaterial(bodymat);
    //mesh
    PolygonMesh* bodyMesh=MeshLoader::readFile("body.msh");
    bodyMesh->setMaterialToAll(bodymat);
    loc.x=-1.6f;
    loc.y=6.3f;
    loc.z=9.0f;
    bodyMesh->translate(loc);
    ret->addMesh(bodyMesh);

    //TODO

    ret->setCamera(render1hCamera());
    return ret;
}


Scene* getTestScene(){
//    return testScene2();
    return render1hScene();
}

