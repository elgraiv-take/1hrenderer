/**
 * @file dataStructure.h
 *
 * @author take
 */

#ifndef DATASTRUCTURE_H_
#define DATASTRUCTURE_H_

#include "root.h"
#include "structDef.h"
#include "largeArray.h"
#include "arrayList.h"

/**
 * @brief 三角形クラス
 * いわゆる三角ポリゴン
 */
class Triangle{
private:
    Vertex* v0; ///< 頂点0
    Vertex* v1; ///< 頂点1
    Vertex* v2; ///< 頂点2

    Vector4D e1;///< 頂点0->頂点1のベクトル
    Vector4D e2;///< 頂点0->頂点2のベクトル
    AABB3D aabb;///< バウンディングボックス

    float area;///< 面積

    Material* material;
public:
    Triangle();
    void setMaterial(Material* _material);
    void setVertex(Vertex* _v0,Vertex* _v1,Vertex* _v2);
    void fix();
    /**
     * @brief レイとポリゴンの交差判定をする
     *
     * @param ray レイオブジェクト
     * @param[out] intersection 結果を格納するオブジェクト
     * @return 交差判定の結果
     * @retval 0 交差しなかった
     * @retval 0以外 交差した
     */
    int getIntersection(const RayObject& ray,RayIntersection& intersection);

    void getPoint(float k,float l,Vector4D& p);
    void getNormal(float k,float l,Vector4D& normal);

    float getArea();

    void brdf(float k,float l,Vector4D& in,Vector4D& out,ColorRGBA& ret);
    Material* getMaterial();
};


class PolygonMesh{
private:
    LargeArray<Triangle>* triangles;
    LargeArray<Vertex>* vertices;
    int triangleNum;
public:
    PolygonMesh();
    ~PolygonMesh();
    void setMaterialToAll(Material* mat);

    void setTriangles(LargeArray<VertexIndices>* t,int num);
    void setVertices(LargeArray<Vertex>* v,int num);
    void rotate(Vector4D& r);
    void translate(Vector4D& v);
    void scale(float s);
    void fix();
    int getTriangleNum();
    LargeArray<Triangle>* getTriangleList();

};

class IFLight{
public:
    virtual void getDirection(Vector4D& pos,Vector4D& direction)=0;
    virtual float getPower(Vector4D& pos)=0;
};

class IFCamera{
public:
    virtual void setDepth(float min,float max)=0;
    virtual void setPosition(Vector4D& _pos)=0;
    virtual void setRotation(Vector4D& _rot)=0;
    virtual void getRayObject(float x,float y,RayObject& ray)=0;
};

class IBackGround{
public:
    virtual ~IBackGround(){}
    virtual void getBGColor(Vector4D& vec,ColorRGBA& bgc)=0;
};

class Scene{
private:
    ArrayList<PolygonMesh*>* meshes;
    ArrayList<IFLight*>* lights;
    ArrayList<Material*>* materials;
    IFCamera* camera;
    IBackGround* bg;
public:
    Scene();
    ~Scene();
    void addMesh(PolygonMesh* m);
    void addLight(IFLight* l);
    void addMaterial(Material* m);
    ArrayList<PolygonMesh*>* getMesheList();
    ArrayList<IFLight*>* getLightList();
    void setCamera(IFCamera* c);
    void setBackGround(IBackGround* _bg);
    IFCamera* getCamera();
//    void getBGColor(Vector4D& vec,ColorRGBA& bgc);
    IBackGround* getBackGround();
};


#endif /* DATASTRUCTURE_H_ */
