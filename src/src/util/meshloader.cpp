/**
 * @file meshloader.cpp
 *
 * @author take
 */
#include "meshloader.h"
#include "largeArray.h"

PolygonMesh* MeshLoader::readFile(const char* file){
    FILE* fp=fopen(file,"rb");
    if(!fp){
        return NULL;
    }
    int vnum;
    fread(&vnum,sizeof(int),1,fp);
    if(vnum<0){
        fclose(fp);
        return NULL;
    }
    LargeArray<Vertex>* vs=new LargeArray<Vertex>(vnum);
    float xyz[6];
    Vertex tmp;
    for(int i=0;i<vnum;i++){
        fread(xyz,sizeof(float),6,fp);
        tmp.co.x=xyz[0];
        tmp.co.y=xyz[1];
        tmp.co.z=xyz[2];
        tmp.normal.x=xyz[3];
        tmp.normal.y=xyz[4];
        tmp.normal.z=xyz[5];
        vs->set(i,tmp);
    }
    int tnum;
    fread(&tnum,sizeof(int),1,fp);
    if(tnum<0){
        SAFE_DELETE_O(vs);
        fclose(fp);
        return NULL;
    }

    LargeArray<VertexIndices>* ts=new LargeArray<VertexIndices>(tnum);
    VertexIndices tvi;
    int v012[3];
    for(int i=0;i<tnum;i++){
        fread(v012,sizeof(int),3,fp);
        tvi.v0=v012[0];
        tvi.v1=v012[1];
        tvi.v2=v012[2];
        ts->set(i,tvi);
    }

    PolygonMesh* ret=new PolygonMesh();
    ret->setVertices(vs,vnum);

    ret->setTriangles(ts,tnum);
    ret->fix();

    SAFE_DELETE_O(vs);
    SAFE_DELETE_O(ts);
    fclose(fp);
    return ret;
}
