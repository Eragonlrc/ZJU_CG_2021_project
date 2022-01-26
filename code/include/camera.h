#pragma once
#include <windows.h>
#include <gl/glu.h>
#include "vector.h"

// �������
class Camera{
public:
    // ���캯������������
    Camera();
    ~Camera();

    // ��������״̬����
    Vector3 getPosition() { return m_Position; }
    Vector3 getView() { return m_Center; }
    Vector3 getUpVector() { return m_UpVector; }
    float   getSpeed() { return m_Speed; }
    float   getFovy() { return m_Fovy; }
    bool    getJumping() { return m_Jumping; }

    // �����ٶ�
    void setSpeed(float speed){
        m_Speed = speed;
    }

    void setJumping(bool jumping) {
        m_Jumping = jumping;
    }

    // �����������λ��, �۲�����������
    void setCamera(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);

    // ��ת���������
    void rotateView(float angle, float X, float Y, float Z);

    // �����������������۲췽��
    void setViewByMouse();

    // �����ƶ������
    void yawCamera(float speed);

    // ǰ���ƶ������
    void moveCamera(float speed);

    // �����ƶ������
    void liftCamera(float v0);

    // x���ƶ������
    void xCamera(float speed);

    // z���ƶ������
    void zCamera(float speed);

    // ���������
    void setLook();

    void updateFovy(float diff);

    void updateHeight(bool check, float floor);
private:
    // ���������
    Vector3 m_Position;
    Vector3 m_Center;
    Vector3 m_UpVector;
    float   m_Speed = 0.05;
    float   m_Fovy = 45.0;
    bool    m_Jumping = false;
    float   velocity = 0;
    float   acceleration = -0.002;
};