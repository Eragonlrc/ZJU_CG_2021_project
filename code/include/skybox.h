#pragma once
#include "texture.h"
#include "vector.h"
#include <string.h>

// ��պ��� 
class SkyBox{
public:
    // ���캯�� 
    SkyBox();
    ~SkyBox();

    // ��ʼ�� 
    bool Init();

    // ��Ⱦ��� 
    void CreateSkyBox(float x, float y, float z, float width, float height, float length);

private:
    static TexLoader texture;   // ��պ����� 
};