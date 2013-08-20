/**
 * @file ployMesh.cpp
 *
 * @author take
 */

#include "dataStructure.h"
#include "vectorMath.h"
#include "brdf.h"

PolygonMesh::PolygonMesh(){
    triangles=NULL;
    vertices=NULL;
    triangleNum=-1;
}
PolygonMesh::~PolygonMesh(){
    SAFE_DELETE_O(triangles);
    SAFE_DELETE_O(vertices);
}
void PolygonMesh::setTriangles(LargeArray<VertexIndices>* t,int num){
    SAFE_DELETE_O(triangles);
    triangleNum=-1;
    triangles=new LargeArray<Triangle>(num);
    Triangle tmp;
    for(int i=0;i<num;i++){
        VertexIndices* vi=t->getp(i);
        tmp.setVertex(vertices->getp(vi->v0),vertices->getp(vi->v1),vertices->getp(vi->v2));
        tmp.setUV(vi->uv0,vi->uv1,vi->uv2);
        triangles->set(i,tmp);
    }
    triangleNum=num;
}
void PolygonMesh::setVertices(LargeArray<Vertex>* v,int num){
    SAFE_DELETE_O(vertices);
    vertices=new LargeArray<Vertex>(num);
    for(int i=0;i<num;i++){
        vertices->set(i,v->get(i));
    }
}
void PolygonMesh::fix(){
    for(int i=0;i<triangleNum;i++){
        triangles->getp(i)->fix();
    }
}
int PolygonMesh::getTriangleNum(){
    return triangleNum;
}
LargeArray<Triangle>* PolygonMesh::getTriangleList(){
    return triangles;
}

void PolygonMesh::setMaterialToAll(Material* mat){
    for(int i=0;i<triangleNum;i++){
        triangles->getp(i)->setMaterial(mat);
    }
}

void PolygonMesh::translate(Vector4D& v){
    int len=vertices->length();
    for(int i=0;i<len;i++){
        Vertex* tv=vertices->getp(i);
        tv->co.x+=v.x;
        tv->co.y+=v.y;
        tv->co.z+=v.z;
    }
    fix();
}

void PolygonMesh::rotate(Vector4D& r){
    int len=vertices->length();
    for(int i=0;i<len;i++){
        Vertex* tv=vertices->getp(i);
        VectorMath::rotXYZ(tv->co,r.x,r.y,r.z,tv->co);
        VectorMath::rotXYZ(tv->normal,r.x,r.y,r.z,tv->normal);
    }
    fix();
}

void PolygonMesh::scale(float s){
    int len=vertices->length();
    for(int i=0;i<len;i++){
        Vertex* tv=vertices->getp(i);
        tv->co.x=tv->co.x*s;
        tv->co.y=tv->co.y*s;
        tv->co.z=tv->co.z*s;
    }
    fix();
}
