#pragma once
#include "texture.h"
#include "vector.h"
#include "camera.h"
#include <string.h>

// 天空盒类 
class CSkyBox{
public:
    // 构造函数 
    CSkyBox();
    ~CSkyBox();

    // 初始化 
    bool Init();

    // 渲染天空 
    void CreateSkyBox(float x, float y, float z, float width, float height, float length);

private:
    static TexLoader texture;   // 天空盒纹理 
};