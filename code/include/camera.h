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

    // 设置速度
    void setSpeed(float speed){
        m_Speed = speed;
    }

    // 设置摄像机的位置, 观察点和向上向量
    void setCamera(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);

    // 旋转摄像机方向
    void rotateView(float angle, float X, float Y, float Z);

    // 根据鼠标设置摄像机观察方向
    void setViewByMouse();

    // 左右摄像机移动
    void yawCamera(float speed);

    // 前后移动摄像机
    void moveCamera(float speed);

    // 上下移动摄像机
    void liftCamera(float speed);

    // x轴移动摄像机
    void xCamera(float speed);

    // z轴移动摄像机
    void zCamera(float speed);

    // 放置摄像机
    void setLook();

private:
    // 摄像机属性
    Vector3        m_Position;
    Vector3        m_Center;
    Vector3        m_UpVector;
    float          m_Speed;         //速度
};