/**
 * @file ppm.cpp
 *
 * @author take
 */

#include "renderer.h"
#include "raytracer.h"
#include "vectorMath.h"
#include "brdf.h"
#include "paramdef.h"
#include <omp.h>
#include "photon.h"

#define DIFFUSE 0
#define BACKGROUND 1
#define LIGHTSOURCE 2
#define NODITECTION 3

int PPMRenderer::raytraceSub(RayIntersection& intersection,RayIntersection& root,int depth){
    Triangle* pt=(Triangle*)intersection.id;
    //    printf("%d",depth);
    Material* mate=pt->getMaterial();
    if(mate->emission){
        root=intersection;
        return LIGHTSOURCE;
    }

    if(depth<1){
        return NODITECTION;
    }

    int vecMode=0;

    if(mate->reflection){
        vecMode=1;
    }else if(mate->refraction){
        vecMode=2;
    }else{
        root=intersection;
        return DIFFUSE;
    }

    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.id=intersection.id;
    ray.ior=intersection.ior;
    pt->getPoint(intersection.k,intersection.l,ray.start);
    Vector4D normal;
    pt->getNormal(intersection.k,intersection.l,normal);
    RayIntersection ri;

    if(vecMode==1){
        VectorMath::reflectionVector(intersection.ray,normal,ray.direct);
    }else if(vecMode==2){
        //        float n1=intersection.ior;
        float n1=1.0f;
        float n2=mate->ior;
        int a=VectorMath::refractionVector(intersection.ray,normal,n1,n2,ray.direct);
        if(a){
        }else{
            ray.ior=mate->ior;
        }
    }

    tracer.getIntersection(ray,ri);
    if(ri.detect){
        return raytraceSub(ri,root,depth-1);
    }else{
        root=ri;
        return BACKGROUND;
    }
}
int PPMRenderer::raytrace(RayIntersection& intersection){
    return raytraceSub(intersection,intersection,10);
}

void PPMRenderer::render(Scene* scene,RawImage& img){
    int h=img.h;
    int w=img.w;
    float offx=w/2.0f;
    float offy=h/2.0f;
    float scl=offx>offy?offx:offy;
    currentScene=scene;
    tracer.setScene(scene);
    cLightSrc=new LightSource(scene);
    RayIntersection* intersection=new RayIntersection[h*w];
    int* pixtype=new int[h*w];

#pragma omp parallel for// num_threads(6)
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            int i=y*w+x;
            float fx=(x-offx)/scl;
            float fy=(y-offy)/scl;
            tracer.getIntersectionFromCamera(fx,fy,intersection[i]);
            if(intersection[i].detect){
                pixtype[i]=raytrace(intersection[i]);
                if(intersection[i].detect){
                    Triangle* pt=(Triangle*)intersection[i].id;
                    pt->getPoint(intersection[i].k,intersection[i].l,intersection[i].point);
                }
            }else{
                pixtype[i]=BACKGROUND;
            }
        }
//        printf("y=%d\n",y);
    }

    int thnum=omp_get_max_threads();
//    int thnum=1;
    omp_set_num_threads(thnum);
    printf("num threads :%d\n",thnum);
    int hitcount=0;
    for(int i=0;i<h*w;i++){
        if(pixtype[i]==DIFFUSE){
            hitcount++;
        }

    }
    RayHitPoint* rhp=new RayHitPoint[hitcount];
    RayHitPoint** screenRhp=new RayHitPoint*[w*h];
    hitcount=0;
    for(int i=0;i<h*w;i++){
        if(pixtype[i]==DIFFUSE){
            rhp[hitcount].ri=intersection[i];
            rhp[hitcount].photonNum=0;
            rhp[hitcount].tempPhotonNum=0;
            rhp[hitcount].radius=PPM_DEFAULT_RADIUS;
            screenRhp[i]=(&(rhp[hitcount]));

//            printf("%x\n",&(rhp[hitcount]));


//            printf("%d\n",&(rhp[hitcount]));
            hitcount++;

        }else{
            screenRhp[i]=NULL;
        }
    }
    rpc=new RayHitPointContainer(rhp,hitcount);
    RayPhotonMap** rpms=new RayPhotonMap*[thnum];
    int* mapCount=new int[thnum];
    for(int i=0;i<thnum;i++){
        rpms[i]=new RayPhotonMap[MAP_SIZE];
        mapCount[i]=0;
    }

    printf("emit photon\n");
#pragma omp parallel for
    for(int i=0;i<PPM_PHOTON_NUM;i++){
        int thNo=omp_get_thread_num();
        emitPhoton(rpms[thNo],&(mapCount[thNo]));
        if(i%1000==0){
            printf("%d %d\n",i,mapCount[thNo]);
        }

        if(mapCount[thNo]>APPLY_SIZE){
#pragma omp critical (update)
            {
                updateRadiance(rpms[thNo],mapCount[thNo]);
            }
            mapCount[thNo]=0;
        }

    }

    for(int i=0;i<thnum;i++){
        updateRadiance(rpms[i],mapCount[i]);
    }


    for(int i=0;i<w*h;i++){
        if(pixtype[i]==DIFFUSE){
            float rate=screenRhp[i]->radius*screenRhp[i]->radius*M_PI*PPM_PHOTON_NUM;
            img.pix[i].r=screenRhp[i]->aphoton.r/rate;
            img.pix[i].g=screenRhp[i]->aphoton.g/rate;
            img.pix[i].b=screenRhp[i]->aphoton.b/rate;
            img.pix[i].r=img.pix[i].r<0.0f?0.0f:img.pix[i].r;
            img.pix[i].g=img.pix[i].g<0.0f?0.0f:img.pix[i].g;
            img.pix[i].b=img.pix[i].b<0.0f?0.0f:img.pix[i].b;
            img.pix[i].r=pow(img.pix[i].r,1/2.20f);
            img.pix[i].g=pow(img.pix[i].g,1/2.20f);
            img.pix[i].b=pow(img.pix[i].b,1/2.20f);
        }else if(pixtype[i]==NODITECTION){
            img.pix[i].r=0.0f;
            img.pix[i].g=0.0f;
            img.pix[i].b=0.0f;
        }else if(pixtype[i]==LIGHTSOURCE){
            float ep=((Triangle*)intersection[i].id)->getMaterial()->emitPower;
            img.pix[i].r=ep;
            img.pix[i].g=ep;
            img.pix[i].b=ep;
        }else if(pixtype[i]==BACKGROUND){
            IBackGround* bg=currentScene->getBackGround();
            if(bg){
                bg->getBGColor(intersection->ray,img.pix[i]);
            }else{
                img.pix[i].r=0.0f;
                img.pix[i].g=0.0f;
                img.pix[i].b=0.0f;
            }
        }
    }
    //SAFE_DELETE_O(rpc);
}
void PPMRenderer::emitPhotonSub(RayIntersection& intersection,RayPhotonMap* map,int* offset,ColorRGBA& color,int depth){
    Triangle* pt=(Triangle*)intersection.id;
    Material* mate=pt->getMaterial();
    if(mate->emission){
//        printf("-----------------------------\n");
        return;
    }
    int vecMode=0;
    if(mate->reflection){
        vecMode=1;
    }else if(mate->refraction){
        vecMode=2;
    }else{
        Photon tmp;
        tmp.color=color;
        tmp.direction=intersection.ray;
        pt->getPoint(intersection.k,intersection.l,tmp.point);
        rpc->applyPhoton(map,tmp,offset,MAP_SIZE,PPM_DEFAULT_RADIUS);
    }
    if(depth<1){
        return;
    }

    RayObject ray;
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.id=intersection.id;
    ray.ior=intersection.ior;
    pt->getPoint(intersection.k,intersection.l,ray.start);
    Vector4D normal;
    pt->getNormal(intersection.k,intersection.l,normal);
    RayIntersection ri;

    if(vecMode==1){
        VectorMath::reflectionVector(intersection.ray,normal,ray.direct);
    }else if(vecMode==2){
        float n1=1.0f;
        float n2=mate->ior;
        int a=VectorMath::refractionVector(intersection.ray,normal,n1,n2,ray.direct);
        if(a){
        }else{
            ray.ior=mate->ior;
        }
    }else{
        VectorMath::setRandomSphereVector(ray.direct);
        float expPow=VectorMath::dot(normal,ray.direct);
        if(expPow<0.0f){
            ray.direct.x*=-1.0f;
            ray.direct.y*=-1.0f;
            ray.direct.z*=-1.0f;
        }
    }

    tracer.getIntersection(ray,ri);
    if(ri.detect){
        ColorRGBA c;
        if(vecMode!=0){
            c.r=color.r;
            c.g=color.g;
            c.b=color.b;
        }else{
            ColorRGBA def;
            pt->brdf(intersection.k,intersection.l,ray.direct,intersection.ray,def);
            c.r=def.r*color.r;
            c.g=def.g*color.g;
            c.b=def.b*color.b;
        }
        emitPhotonSub(ri,map,offset,c,depth-1);

    }
}
void PPMRenderer::emitPhoton(RayPhotonMap* map,int* offset){
    RayObject ray;
    RayIntersection ri;
    ColorRGBA light;
    cLightSrc->getRandumPhoton(ray,light);
    tracer.getIntersection(ray,ri);
    if(ri.detect){
        emitPhotonSub(ri,map,offset,light,PPM_MAX_PHOTON_DEPTH);
    }
}

void PPMRenderer::updateRadiance(RayPhotonMap* map,int num){
    ColorRGBA tc;
    for(int i=0;i<num;i++){
        RayHitPoint* ray=map[i].ray;
        if(VectorMath::distance(map[i].photon.point,ray->ri.point)<ray->radius){
            ray->tempPhotonNum++;
            ((Triangle*)(ray->ri.id))->brdf(ray->ri.k,ray->ri.l,map[i].photon.direction,ray->ri.ray,tc);
//            printf("%f\n",tc.r);
            ray->tempAphoton.r+=(tc.r*map[i].photon.color.r);
            ray->tempAphoton.g+=(tc.g*map[i].photon.color.g);
            ray->tempAphoton.b+=(tc.b*map[i].photon.color.b);
        }
    }
    for(int i=0;i<num;i++){
        RayHitPoint* ray=map[i].ray;
        if(ray->tempPhotonNum){
            int n=ray->photonNum;
            int m=ray->tempPhotonNum;
            float namnm=((float)n+PPM_ALPHA*m)/(float)(n+m);
            float newR=ray->radius*sqrt(namnm);
            ray->radius=newR;
            float nr=(ray->aphoton.r+ray->tempAphoton.r)*namnm;
            float ng=(ray->aphoton.g+ray->tempAphoton.g)*namnm;
            float nb=(ray->aphoton.b+ray->tempAphoton.b)*namnm;
//            printf("%d\n",ray->photonNum);
            ray->aphoton.r=nr;
            ray->aphoton.g=ng;
            ray->aphoton.b=nb;
//            printf("%f\n",ray->aphoton.r);
            ray->tempAphoton.r=0.0f;
            ray->tempAphoton.g=0.0f;
            ray->tempAphoton.b=0.0f;
            ray->photonNum=n+m;
            ray->tempPhotonNum=0;
//            printf("------%x\n",ray);
        }
    }
}

//----------------------------------------------------------------

RayHitPointContainer::RayHitPointContainer(RayHitPoint* _hitpoints,int n){
    hitpoints=_hitpoints;
    num=n;
    sort[0]=new RayHitPoint*[n];
    sort[1]=new RayHitPoint*[n];
    sort[2]=new RayHitPoint*[n];
    kdTree=new RayHitPoint*[n];
    childrenCounter=new int[n];
    divAxis=new int[n];
    for(int i=0;i<n;i++){
//        if(i<5){
//            printf("%x\n",&(hitpoints[i]));
//        }


        hitpoints[i].kdtIndex=0;
        sort[0][i]=&(hitpoints[i]);
        sort[1][i]=&(hitpoints[i]);
        sort[2][i]=&(hitpoints[i]);
        childrenCounter[i]=1;
        kdTree[i]=0;
    }
    for(int i=n-1;i>0;i--){
        int pi=(i-1)/2;
        childrenCounter[pi]+=childrenCounter[i];
    }
    printf("sort\n");
    sortPoint(sort[AXIS_X],AXIS_X);
//    for(int i=1;i<num;i++){
//        printf("%f-------%f\n",sort[AXIS_X][i]->ri.point.xyzw[AXIS_X],sort[AXIS_X][i-1]->ri.point.xyzw[AXIS_X]);
//        if(sort[AXIS_X][i]->ri.point.xyzw[AXIS_X]<sort[AXIS_X][i-1]->ri.point.xyzw[AXIS_X]){
//            printf("%f-------%f\n",sort[AXIS_X][i]->ri.point.xyzw[AXIS_X],sort[AXIS_X][i-1]->ri.point.xyzw[AXIS_X]);
//        }
//    }
    sortPoint(sort[AXIS_Y],AXIS_Y);
    sortPoint(sort[AXIS_Z],AXIS_Z);
    printf("sort comp\n");
    constractKdt(0,AXIS_X);
    for(int i=0;i<n;i++){
        kdTree[hitpoints[i].kdtIndex]=(&(hitpoints[i]));
        //printf("%x %x \n",kdTree[hitpoints[i].kdtIndex],(&(hitpoints[i])));
    }
}

RayHitPointContainer::~RayHitPointContainer(){
//    for(int i=0;i<num;i++){
//        printf("%f\n",hitpoints[i].aphoton.r);
//    }
    SAFE_DELETE_A(sort[0]);
    SAFE_DELETE_A(sort[1]);
    SAFE_DELETE_A(sort[2]);
    SAFE_DELETE_A(kdTree);
    SAFE_DELETE_A(childrenCounter);
    SAFE_DELETE_A(divAxis);
}

void RayHitPointContainer::constractKdt(int index,int axis){
    int* counter=new int[num];
    for(int i=0;i<num;i++){
        counter[i]=0;
    }
    index=0;
    axis=AXIS_X;
    while(index<num){
        int last=index*2+1;
        if(last>num){
            last=num;
        }

        for(int i=0;i<num;i++){
            RayHitPoint* hp=sort[axis][i];
            int in=hp->kdtIndex;
            if(in<index){
                continue;
            }
            int li=in*2+1;
            if(li>=num){
                continue;
            }
            if(counter[in]<childrenCounter[li]){
                hp->kdtIndex=li;
            }else if(counter[in]>childrenCounter[li]){
                hp->kdtIndex=li+1;
            }
            counter[in]++;
        }

        for(int i=index;i<last;i++){
            divAxis[i]=axis;
        }
        axis=NEXT_AXIS(axis);
        index=last;
    }
    SAFE_DELETE_A(counter);
}

//void RayHitPointContainer::constractKdt(int index,int axis){
//    divAxis[index]=axis;
//    int lc=index*2+1;
//    int lnum=0;
//    if(!(lc<num)){
////        printf("%d\n",index);
//        return;
//    }else{
//        lnum=childrenCounter[lc];
//    }
//    int rc=index*2+2;
//    int rnum=0;
//    if(rc<num){
//        rnum=childrenCounter[rc];
//    }
//    int count=0;
//    int sindex=lc;
//    for(int i=0;i<num;i++){
//        RayHitPoint* tmp=sort[axis][i];
////        if(index==0){
////            printf("----------------------------%d--%d--%d\n",tmp->kdtIndex,index,i);
////        }
//
//        if(tmp->kdtIndex==index){
//
//            if(count==lnum){
//                sindex=rc;
//            }else{
//                tmp->kdtIndex=sindex;
//            }
//            count++;
//            if(!(count<childrenCounter[index])){
//                break;
//            }
//        }
//    }
//    constractKdt(lc,NEXT_AXIS(axis));
//    constractKdt(rc,NEXT_AXIS(axis));
//}

void RayHitPointContainer::heepup(RayHitPoint** array,int axis,int index){
    if(index==0){
        return;
    }
    int pindex=(index-1)/2;
    if(array[index]->ri.point.xyzw[axis]>array[pindex]->ri.point.xyzw[axis]){
        RayHitPoint* tmp=array[index];
        array[index]=array[pindex];
        array[pindex]=tmp;
        heepup(array,axis,pindex);
    }
}
void RayHitPointContainer::heepdown(RayHitPoint** array,int axis,int index,int size){
    int lc=index*2+1;

    if(!(lc<size)){
        return;
    }

    float max=array[index]->ri.point.xyzw[axis];
    int nindex=index;
    if(max<array[lc]->ri.point.xyzw[axis]){
        max=array[lc]->ri.point.xyzw[axis];
        nindex=lc;
    }
    int rc=index*2+2;
    if((rc<size)&&(max<array[rc]->ri.point.xyzw[axis])){
        nindex=rc;
    }

    if(nindex!=index){
        RayHitPoint* tmp=array[index];
        array[index]=array[nindex];
        array[nindex]=tmp;
        heepdown(array,axis,nindex,size);
    }
}

void RayHitPointContainer::sortPoint(RayHitPoint** array,int axis){
    for(int i=1;i<num;i++){
//        printf("%d\n",i);
        heepup(array,axis,i);
    }
    for(int i=num-1;i>0;i--){
        RayHitPoint* tmp=array[i];
        array[i]=array[0];
        array[0]=tmp;
        heepdown(array,axis,0,i);
    }
}
void RayHitPointContainer::applyPhotonSub(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius,int index){

    int axis=divAxis[index];
    float axisDist=photon.point.xyzw[axis]-kdTree[index]->ri.point.xyzw[axis];
    int lc=index*2+1;
    int rc=index*2+2;
    float d=VectorMath::distance(photon.point,kdTree[index]->ri.point);
//    printf("----------------------%f %f %f\n",photon.point.x,photon.point.y,photon.point.z);
//    printf("-----------------------------%f\n",d);
    if(d<radius){
        int of=(*offset);
        if(of<max){
            rpm[of].ray=kdTree[index];
//            printf("%x-%x \n",rpm[(*offset)].ray,kdTree[index]);
            rpm[of].photon=photon;
//            printf("-----------------------------%f\n",d);
            (*offset)++;
        }
    }
    if(axisDist<0){
        if(lc<num){
            applyPhotonSub(rpm,photon,offset,max,radius,lc);
        }
        if(axisDist>-radius){
            if(rc<num){
                applyPhotonSub(rpm,photon,offset,max,radius,rc);
            }
        }
    }else{
        if(rc<num){
            applyPhotonSub(rpm,photon,offset,max,radius,rc);
        }
        if(axisDist<radius){
            if(lc<num){
                applyPhotonSub(rpm,photon,offset,max,radius,lc);
            }
        }
    }
}
void RayHitPointContainer::applyPhoton(RayPhotonMap* rpm,Photon& photon,int* offset,int max,float radius){
//    printf("%f,%f,%f\n",photon.point.x,photon.point.y,photon.point.z);
    applyPhotonSub(rpm,photon,offset,max,radius,0);
}

//------------------------------------------------------------------------------------

LightSource::LightSource(Scene* scene){
    int lnum=0;
    ArrayList<PolygonMesh*>* ms=scene->getMesheList();
    int n=ms->getSize();
    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct->getMaterial()->emission){
                lnum++;
            }
        }
    }
    num=lnum;
    sources=new Triangle*[lnum];
    lnum=0;
    for(int i=0;i<n;i++){
        PolygonMesh* pm=ms->get(i);
        LargeArray<Triangle>* ts=pm->getTriangleList();
        int m=ts->length();
        for(int j=0;j<m;j++){
            Triangle* ct=ts->getp(j);
            if(ct->getMaterial()->emission){
                sources[lnum]=ct;
                lnum++;
            }
        }
    }
    areas=new float[num];
    float sum=0.0f;
    for(int i=0;i<num;i++){
        sum+=sources[i]->getArea();
        areas[i]=sum;
    }
    for(int i=0;i<num;i++){
        areas[i]/=sum;
    }
}
LightSource::~LightSource(){
    SAFE_DELETE_A(sources);
    SAFE_DELETE_A(areas);
}
void LightSource::getRandumPhoton(RayObject& ray,ColorRGBA& color){
    float r=Xorshift::nextf();
    int index=0;
    for(int i=0;i<num;i++){
        if(r<areas[i]){
            index=i;
            break;
        }
    }
    float k=Xorshift::nextf();
    float l=Xorshift::nextf();
    if(k+l>1){
        k=1.0f-k;
        l=1.0f-l;
    }

    sources[index]->getPoint(k,l,ray.start);
    ray.minDepth=0.0f;
    ray.maxDepth=100.0f;
    ray.ior=1.0f;
    VectorMath::setRandomSphereVector(ray.direct);
//    Vector4D no;
//    sources[index]->getNormal(k,l,no);
//    float dot=VectorMath::dot(ray.direct,no);
//    if(dot<0){
//        ray.direct.x*=-1;
//        ray.direct.y*=-1;
//        ray.direct.z*=-1;
//    }
    ray.id=sources[index];
    color.r=sources[index]->getMaterial()->emitPower;
    color.g=sources[index]->getMaterial()->emitPower;
    color.b=sources[index]->getMaterial()->emitPower;

//    VectorMath::dot(ray.direct)
//    if()
}
