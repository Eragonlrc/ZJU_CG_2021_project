#pragma once
#include "texture.h"
#include "vector.h"
#include <string.h>

#define BOX_SIZE 1024

// 天空盒类 
class SkyBox{
public:
    // 构造函数 
    SkyBox();
    ~SkyBox();

    // 初始化 
    bool init();

    // 渲染天空 
    void createSkyBox(float x, float y, float z, float width, float height, float length);

private:
    static TexLoader texture;   // 天空盒纹理 
};