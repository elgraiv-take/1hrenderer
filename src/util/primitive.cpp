/**
 * @file primitive.cpp
 *
 * @author take
 */

#include "primitive.h"

PolygonMesh* PrimitiveGen::createPlane(float w,float h){
    LargeArray<Vertex> vs(4);
    LargeArray<VertexIndices> ts(2);
    Vertex v;
    v.co.z=0.0f;
    v.normal.x=v.normal.y=0.0f;
    v.normal.z=1.0f;

    v.co.x=0.5f*w;
    v.co.y=0.5f*h;
    vs.set(0,v);

    v.co.x*=-1.0f;
    vs.set(1,v);

    v.co.y*=-1.0f;
    vs.set(2,v);

    v.co.x*=-1.0f;
    vs.set(3,v);


    VertexIndices t;
    t.v0=0;t.v1=1;t.v2=2;
    ts.set(0,t);
//    t.setVertex(vs.getp(2),vs.getp(3),vs.getp(0));
    t.v0=2;t.v1=3;t.v2=0;
    ts.set(1,t);
    PolygonMesh* ret=new PolygonMesh();
    ret->setVertices(&vs,4);

    ret->setTriangles(&ts,2);
    ret->fix();
    return ret;
}
PolygonMesh* PrimitiveGen::createSphere(float size,int divh,int divv){
    int vnum=(divh-1)*divv+2;
    LargeArray<Vertex>* vs=new LargeArray<Vertex>(vnum);
    int tnum=(divh-1)*divv*2;
    LargeArray<VertexIndices>* ts=new LargeArray<VertexIndices>(tnum);

    size*=0.5f;

    Vertex v;
    v.co.x=0.0f;
    v.co.y=0.0f;
    v.co.z=size;
    v.normal.x=0.0f;
    v.normal.y=0.0f;
    v.normal.z=1.0f;
    int i=0;
    vs->set(i++,v);
    for(int hi=1;hi<divh;hi++){
        float phi=(M_PI*hi)/divh;
        for(int vj=0;vj<divv;vj++){
            float the=(M_PI*2.0f*vj)/divv;
            float x=cos(the)*sin(phi);
            float y=sin(the)*sin(phi);
            float z=cos(phi);
            v.normal.x=x;
            v.normal.y=y;
            v.normal.z=z;
            v.co.x=x*size;
            v.co.y=y*size;
            v.co.z=z*size;
            printf("%f\n",(x*x+y*y+z*z));
            vs->set(i++,v);
        }
    }
    v.co.x=0.0f;
    v.co.y=0.0f;
    v.co.z=-size;
    v.normal.x=0.0f;
    v.normal.y=0.0f;
    v.normal.z=-1.0f;
    vs->set(i++,v);

    VertexIndices t;
    i=0;
    for(int vi=0;vi<divv;vi++){
        int v0=vi+1;
        int v1=((vi+1)%divv)+1;
        t.v0=v0;t.v1=v1;t.v2=0;
        ts->set(i++,t);
    }
    for(int hi=0;hi<divh-2;hi++){
        int offset=hi*divv+1;
        for(int vi=0;vi<divv;vi++){
            int v0=((vi+1)%divv)+offset;
            int v1=vi+offset;
            int v2=vi+offset+divv;
            int v3=((vi+1)%divv)+offset+divv;
            t.v0=v0;t.v1=v1;t.v2=v2;
            ts->set(i++,t);
            t.v0=v2;t.v1=v3;t.v2=v0;
            ts->set(i++,t);
        }
    }
    int offset=(divh-2)*divv+1;
    for(int vi=0;vi<divv;vi++){
        int v0=((vi+1)%divv)+offset;
        int v1=vi+offset;
        t.v0=v0;t.v1=v1;t.v2=vnum-1;
        ts->set(i++,t);
    }

    PolygonMesh* ret=new PolygonMesh();
    ret->setVertices(vs,vnum);
    ret->setTriangles(ts,tnum);
    ret->fix();
    SAFE_DELETE_O(vs);
    SAFE_DELETE_O(ts);
    return ret;
}

