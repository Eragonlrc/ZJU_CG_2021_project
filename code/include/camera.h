#pragma once
#include <windows.h>
#include <gl/glu.h>
#include "vector.h"

// 摄像机类
class Camera{
public:
    // 构造函数和析构函数
    Camera();
    ~Camera();

    // 获得摄像机状态方法
    Vector3 getPosition() { return m_Position; }
    Vector3 getView() { return m_Center; }
    Vector3 getUpVector() { return m_UpVector; }
    float   getSpeed() { return m_Speed; }
    float   getFovy() { return m_Fovy; }
    bool    getJumping() { return m_Jumping; }

    // 设置速度
    void setSpeed(float speed){
        m_Speed = speed;
    }

    void setJumping(bool jumping) {
        m_Jumping = jumping;
    }

    // 设置摄像机的位置, 观察点和向上向量
    void setCamera(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);

    // 旋转摄像机方向
    void rotateView(float angle, float X, float Y, float Z);

    // 根据鼠标设置摄像机观察方向
    void setViewByMouse();

    // 左右移动摄像机
    void yawCamera(float speed);

    // 前后移动摄像机
    void moveCamera(float speed);

    // 上下移动摄像机
    void liftCamera(float v0);

    // x轴移动摄像机
    void xCamera(float speed);

    // z轴移动摄像机
    void zCamera(float speed);

    // 放置摄像机
    void setLook();

    void updateFovy(float diff);

    void updateHeight(bool check, float floor);
private:
    // 摄像机属性
    Vector3 m_Position;
    Vector3 m_Center;
    Vector3 m_UpVector;
    float   m_Speed = 0.05;
    float   m_Fovy = 45.0;
    bool    m_Jumping = false;
    float   velocity = 0;
    float   acceleration = -0.002;
};