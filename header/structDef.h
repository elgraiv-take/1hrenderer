/**
 * @file structDef.h
 *
 * @author take
 */

#ifndef STRUCTDEF_H_
#define STRUCTDEF_H_

#include "root.h"

/**
 * @brief ベクトル構造体(4次元)
 *
 */
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
    };///< 表記に柔軟性を持たせるための共有体
    /**
     * @brief デフォルトコンストラクタ
     * (0,0,0,1)で初期化
     *
     */
    Vector4D();
    /**
     * @brief 4引数のコンストラクタ
     * (_x,_y,_z,_w)で初期化
     *
     * @param _x xの初期値
     * @param _y yの初期値
     * @param _z zの初期値
     * @param _w wの初期値
     */
    Vector4D(float _x,float _y,float _z,float _w);
    /**
     * @brief 3引数のコンストラクタ
     * (_x,_y,_z,1)で初期化
     *
     * @param _x xの初期値
     * @param _y yの初期値
     * @param _z zの初期値
     */
    Vector4D(float _x,float _y,float _z);
    /**
     * @brief 配列引数のコンストラクタ
     *
     * @param initValues ベクトルの初期値
     */
    Vector4D(const float initValues[]);
};

/**
 * @brief 色構造体
 */
typedef Vector4D ColorRGBA;

/**
 * @brief 行列構造体(4x4)
 *
 */
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
    /**
     * @brief デフォルトコンストラクタ
     * 単位行列で初期化
     *
     */
    Matrix4D();
    /**
     * @brief 1次元配列引数のコンストラクタ
     * aij = initValues[i*4+j]で初期化
     *
     * @param initValues 行列の初期値
     */
    Matrix4D(const float initValues[]);
    /**
     * @brief 2次元配列引数のコンストラクタ
     * aij = initValues[i][j]で初期化
     *
     * @param initValues 行列の初期値
     */
    Matrix4D(const float initValues[4][4]);
    /**
     * @brief 16引数のコンストラクタ
     * aij=_aijで初期化
     */
    Matrix4D(
            float _a11,float _a12,float _a13,float _a14,
            float _a21,float _a22,float _a23,float _a24,
            float _a31,float _a32,float _a33,float _a34,
            float _a41,float _a42,float _a43,float _a44
            );
};

/**
 * @brief 頂点構造体
 */
struct Vertex{
    /**
     * @brief 頂点の座標
     */
    Vector4D co;
    /**
     * @brief 頂点法線
     */
    Vector4D normal;
};

struct VertexIndices{
    int v0,v1,v2;
};

/**
 * @brief 3次元の軸並行バウンディングボックス．
 * あくしすあらいんどばうんでぃんぐぼっくす．
 * 略してAABB．
 */
struct AABB3D{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
};

/**
 * @brief レイオブジェクト構造体．
 * レイトレースのレイ．
 */
struct RayObject{
    Vector4D start;///< 視点位置
    Vector4D direct;///< 視線方向(ノルムは1)
    void* id;///<前回交差したオブジェクト．交差判定の除外対象
    float ior;///<空間の屈折率
    float maxDepth;///< 奥行き最大値
    float minDepth;///< 奥行き最小値
};

/**
 * @brief レイの交点オブジェクト．
 * レイトレースの結果ポリゴントレイが交差するかどうか
 * 交差する場合はその情報を格納する．
 */
struct RayIntersection{
    float depth;///<交点の奥行き値
    float k;///<交点位置k
    float l;///<交点位置l
    int detect;///<交点があったかどうか
    Vector4D point;///<交点座標
    Vector4D normal;///<法線
    Vector4D ray;///<レイの入射角
    float ior;///<通過してきた物体の屈折率
    void* id;///<交差したオブジェクト
};

struct Material{
    BRDF* brdf;
    int emission;
    float emitPower;
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
