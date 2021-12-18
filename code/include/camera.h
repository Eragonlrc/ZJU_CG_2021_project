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

    // �����ٶ�
    void setSpeed(float speed){
        m_Speed = speed;
    }

    // �����������λ��, �۲�����������
    void setCamera(float positionX, float positionY, float positionZ,
        float viewX, float viewY, float viewZ,
        float upVectorX, float upVectorY, float upVectorZ);

    // ��ת���������
    void rotateView(float angle, float X, float Y, float Z);

    // �����������������۲췽��
    void setViewByMouse();

    // ����������ƶ�
    void yawCamera(float speed);

    // ǰ���ƶ������
    void moveCamera(float speed);

    // �����ƶ������
    void liftCamera(float speed);

    // x���ƶ������
    void xCamera(float speed);

    // z���ƶ������
    void zCamera(float speed);

    // ���������
    void setLook();

private:
    // ���������
    Vector3        m_Position;
    Vector3        m_Center;
    Vector3        m_UpVector;
    float          m_Speed;         //�ٶ�
};