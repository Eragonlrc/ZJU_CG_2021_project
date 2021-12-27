#pragma once
#include "texture.h"
#include "vector.h"
#include <string.h>

#define BOX_SIZE 1024

// ��պ��� 
class SkyBox{
public:
    // ���캯�� 
    SkyBox();
    ~SkyBox();

    // ��ʼ�� 
    bool init();

    // ��Ⱦ��� 
    void createSkyBox(float x, float y, float z, float width, float height, float length);

private:
    static TexLoader texture;   // ��պ����� 
};