/**
 * @file structDef.h
 *
 * @author take
 */

#ifndef STRUCTDEF_H_
#define STRUCTDEF_H_

#include "root.h"
struct Vector2D{
    union {
        struct {
            float x,y;
        };
        float xy[2];
        struct {
            float u,v;
        };
        float uv[4];
    };
    Vector2D(){
        u=v=0.0f;
    }
};

typedef Vector2D VectorUV;

struct Vector4D{
    union {
        struct {
            float x,y,z,w;
        };
        float xyzw[4];
        struct {
            float r,g,b,a;
        };
        float rgba[4];
    };

    Vector4D();
    Vector4D(float _x,float _y,float _z,float _w);
    Vector4D(float _x,float _y,float _z);
    Vector4D(const float initValues[]);
};

typedef Vector4D ColorRGBA;

struct Matrix4D{
    union {
        struct {
            float a11,a12,a13,a14;
            float a21,a22,a23,a24;
            float a31,a32,a33,a34;
            float a41,a42,a43,a44;
        };
        float a[16];
        float a4x4[4][4];
    };
    Matrix4D();
    Matrix4D(const float initValues[]);
    Matrix4D(const float initValues[4][4]);
    Matrix4D(float _a11,float _a12,float _a13,float _a14,float _a21,float _a22,float _a23,float _a24,float _a31,float _a32,float _a33,float _a34,float _a41,float _a42,float _a43,float _a44);
};

struct Vertex{
    Vector4D co;
    Vector4D normal;
};

struct VertexIndices{
    int v0,v1,v2;
    VectorUV uv0,uv1,uv2;
};

struct AABB3D{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    Vector4D center;
    AABB3D(){
        xmin=0.0f;
        xmax=0.0f;
        ymin=0.0f;
        ymax=0.0f;
        zmin=0.0f;
        zmax=0.0f;
    }
};

struct RayObject{
    Vector4D start;
    Vector4D direct;
    void* id;
    float ior;
    float maxDepth;
    float minDepth;
};

enum DataType{
    POLYGON_MESH, IS_HAIR
};
class Renderable;
struct RayIntersection{
    float depth;
    float k;
    float l;
    int detect;
    Vector4D point;
    Vector4D normal;
    Vector4D ray;
    float ior;
    Renderable* id;
    DataType type;
};

struct Material{
    BRDF* brdf;
    int emission;
    float emitPower;
    ColorRGBA emitColor;
    int reflection;
    int refraction;
    float ior;
    Material();
    ~Material();
};

struct RawImage{
    ColorRGBA* pix;
    int w;
    int h;
    RawImage(int _w,int _h);
    ~RawImage();
};

#endif /* STRUCTDEF_H_ */
