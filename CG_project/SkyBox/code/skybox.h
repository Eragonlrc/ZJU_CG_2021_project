#pragma once
#include "texture.h"
#include "vector.h"
#include "camera.h"
#include <string.h>

// ��պ��� 
class CSkyBox{
public:
    // ���캯�� 
    CSkyBox();
    ~CSkyBox();

    // ��ʼ�� 
    bool Init();

    // ��Ⱦ��� 
    void CreateSkyBox(float x, float y, float z, float width, float height, float length);

private:
    static TexLoader texture;   // ��պ����� 
};