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

/**
 * @brief �x�N�g���\����(4����)
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
    }; ///< �\�L�ɏ_����������邽�߂̋��L��
    /**
     * @brief �f�t�H���g�R���X�g���N�^
     * (0,0,0,1)�ŏ���
     *
     */
    Vector4D();
    /**
     * @brief 4��̃R���X�g���N�^
     * (_x,_y,_z,_w)�ŏ���
     *
     * @param _x x�̏���l
     * @param _y y�̏���l
     * @param _z z�̏���l
     * @param _w w�̏���l
     */
    Vector4D(float _x,float _y,float _z,float _w);
    /**
     * @brief 3��̃R���X�g���N�^
     * (_x,_y,_z,1)�ŏ���
     *
     * @param _x x�̏���l
     * @param _y y�̏���l
     * @param _z z�̏���l
     */
    Vector4D(float _x,float _y,float _z);
    /**
     * @brief �z���̃R���X�g���N�^
     *
     * @param initValues �x�N�g���̏���l
     */
    Vector4D(const float initValues[]);
};

/**
 * @brief �F�\����
 */
typedef Vector4D ColorRGBA;

/**
 * @brief �s��\����(4x4)
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
     * @brief �f�t�H���g�R���X�g���N�^
     * �P�ʍs��ŏ���
     *
     */
    Matrix4D();
    /**
     * @brief 1�����z���̃R���X�g���N�^
     * aij = initValues[i*4+j]�ŏ���
     *
     * @param initValues �s��̏���l
     */
    Matrix4D(const float initValues[]);
    /**
     * @brief 2�����z���̃R���X�g���N�^
     * aij = initValues[i][j]�ŏ���
     *
     * @param initValues �s��̏���l
     */
    Matrix4D(const float initValues[4][4]);
    /**
     * @brief 16��̃R���X�g���N�^
     * aij=_aij�ŏ���
     */
    Matrix4D(float _a11,float _a12,float _a13,float _a14,float _a21,float _a22,float _a23,float _a24,float _a31,float _a32,float _a33,float _a34,float _a41,float _a42,float _a43,float _a44);
};

/**
 * @brief ���_�\����
 */
struct Vertex{
    /**
     * @brief ���_�̍��W
     */
    Vector4D co;
    /**
     * @brief ���_�@��
     */
    Vector4D normal;

//    VectorUV uv;

};

struct VertexIndices{
    int v0,v1,v2;
    VectorUV uv0,uv1,uv2;
};

/**
 * @brief 3�����̎����s�o�E���f�B���O�{�b�N�X�D
 * �����������炢��ǂ΂���ł��񂮂ڂ������D
 * ������AABB�D
 */
struct AABB3D{
    float xmin;
    float xmax;
    float ymin;
    float ymax;
    float zmin;
    float zmax;
    Vector4D center;
    AABB3D(){
        xmin=-123.0f;
        xmax=-124.0f;
        ymin=-125.0f;
        ymax=-126.0f;
        zmin=-127.0f;
        zmax=-128.0f;
    }
};

/**
 * @brief ���C�I�u�W�F�N�g�\���́D
 * ���C�g���[�X�̃��C�D
 */
struct RayObject{
    Vector4D start; ///< ���_�ʒu
    Vector4D direct; ///< ������(�m������1)
    void* id; ///<�O����������I�u�W�F�N�g�D��������̏��O�Ώ�
    float ior; ///<��Ԃ̋�ܗ�
    float maxDepth; ///< ���s���ő�l
    float minDepth; ///< ���s���ŏ��l
};

enum DataType{
    POLYGON_MESH, IS_HAIR
};
class Renderable;
/**
 * @brief ���C�̌�_�I�u�W�F�N�g�D
 * ���C�g���[�X�̌��ʃ|���S���g���C���������邩�ǂ���
 * ��������ꍇ�͂��̏����i�[����D
 */
struct RayIntersection{
    float depth; ///<��_�̉��s���l
    float k; ///<��_�ʒuk
    float l; ///<��_�ʒul
    int detect; ///<��_�����������ǂ���
    Vector4D point; ///<��_���W
    Vector4D normal; ///<�@��
    Vector4D ray; ///<���C�̓�ˊp
    float ior; ///<�ʉ߂��Ă������̂̋�ܗ�
    Renderable* id; ///<���������I�u�W�F�N�g
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
