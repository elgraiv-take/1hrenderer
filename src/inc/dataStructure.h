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
#include "renderable.h"
#include "ishair.h"

/**
 * @brief �ｽO�ｽp�ｽ`�ｽN�ｽ�ｽ�ｽX
 * �ｽ�ｽ�ｽ�ｽ�ｽ�ｽO�ｽp�ｽ|�ｽ�ｽ�ｽS�ｽ�ｽ
 */
class Triangle:public Renderable{
private:
    Vertex* v0; ///< �ｽ�ｽ�ｽ_0
    Vertex* v1; ///< �ｽ�ｽ�ｽ_1
    Vertex* v2; ///< �ｽ�ｽ�ｽ_2

    Vector4D e1;///< �ｽ�ｽ�ｽ_0->�ｽ�ｽ�ｽ_1�ｽﾌベ�ｽN�ｽg�ｽ�ｽ
    Vector4D e2;///< �ｽ�ｽ�ｽ_0->�ｽ�ｽ�ｽ_2�ｽﾌベ�ｽN�ｽg�ｽ�ｽ

    VectorUV uv0,uv1,uv2;

    float area;///< �ｽﾊ撰ｿｽ

    Material* material;
public:
    AABB3D aabb;///< �ｽo�ｽE�ｽ�ｽ�ｽf�ｽB�ｽ�ｽ�ｽO�ｽ{�ｽb�ｽN�ｽX
    Triangle();
    void setMaterial(Material* _material);
    void setVertex(Vertex* _v0,Vertex* _v1,Vertex* _v2);
    void setUV(VectorUV& _uv0,VectorUV& _uv1,VectorUV& _uv2);
    void fix();
    /**
     * @brief �ｽ�ｽ�ｽC�ｽﾆポ�ｽ�ｽ�ｽS�ｽ�ｽ�ｽﾌ鯉ｿｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ
     *
     * @param ray �ｽ�ｽ�ｽC�ｽI�ｽu�ｽW�ｽF�ｽN�ｽg
     * @param[out] intersection �ｽ�ｽ�ｽﾊゑｿｽ�ｽi�ｽ[�ｽ�ｽ�ｽ�ｽI�ｽu�ｽW�ｽF�ｽN�ｽg
     * @return �ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾌ鯉ｿｽ�ｽ�ｽ
     * @retval 0 �ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽﾈゑｿｽ�ｽ�ｽ�ｽ�ｽ
     * @retval 0�ｽﾈ外 �ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ�ｽ
     */
    int getIntersection(const RayObject& ray,RayIntersection& intersection);

    void getPoint(const RayIntersection& intersection,Vector4D& p);
    void getUV(const RayIntersection& intersection,VectorUV& uv);
    int getNormal(const RayIntersection& intersection,Vector4D& normal);
    void getTangent(const RayIntersection& intersection,Vector4D& tangent);

    void getPoint(float k,float l,Vector4D& p);
    void getNormal(float k,float l,Vector4D& normal);

    float getArea();

//    void brdf(float k,float l,Vector4D& in,Vector4D& out,ColorRGBA& ret);
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
    virtual ~IFLight(){}
    virtual void getDirection(Vector4D& pos,Vector4D& direction)=0;
    virtual float getPower(Vector4D& pos)=0;
};

class IFCamera{
public:
    virtual ~IFCamera(){}
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
    ArrayList<IsHair*>* ishairs;
    IFCamera* camera;
    IBackGround* bg;
public:
    Scene();
    ~Scene();
    void addMesh(PolygonMesh* m);
    void addLight(IFLight* l);
    void addMaterial(Material* m);
    void addIsHair(IsHair* h);
    ArrayList<PolygonMesh*>* getMesheList();
    ArrayList<IFLight*>* getLightList();
    ArrayList<IsHair*>* getIsHairList();
    void setCamera(IFCamera* c);
    void setBackGround(IBackGround* _bg);
    IFCamera* getCamera();
//    void getBGColor(Vector4D& vec,ColorRGBA& bgc);
    IBackGround* getBackGround();
};


#endif /* DATASTRUCTURE_H_ */
