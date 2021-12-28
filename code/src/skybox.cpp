#include "SkyBox.h"

TexLoader SkyBox::texture(6);

SkyBox::SkyBox(){
}

SkyBox::~SkyBox(){
}

// 天空盒初始化 
bool SkyBox::init(){
    char filename[128]; // 用来保存文件名 
    char bmpName[6][128] = { "back","front","bottom","top","left","right" };
    texture.genTex();
    for (int i = 0; i < 6; i++){
        sprintf_s(filename, "textures/box/");
        strcat_s(filename, bmpName[i]);
        strcat_s(filename, ".bmp");
        texture.loadTex(i, filename);
    }
    return true;

}

// 构造天空盒 
void  SkyBox::createSkyBox(float x, float y, float z, float box_width, float box_height, float box_length){
    // 获得场景中光照状态 
    GLboolean lp;
    glGetBooleanv(GL_LIGHT0, &lp);

    // 计算天空盒长 宽 高 
    float width = BOX_SIZE * box_width;
    float height = BOX_SIZE * box_height;
    float length = BOX_SIZE * box_length;

    // 半长 半宽 半高
    float w = width / 2;
    float h = height / 2;
    float l = length / 2;

    glDisable(GL_LIGHT0);            // 关闭光照 

    // 开始绘制 
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    // 绘制背面 
    texture.bindTex(0);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y - h, z - l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - w, y + h, z - l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w, y - h, z - l);

    glEnd();

    // 绘制前面 
    texture.bindTex(1);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z + l);

    glEnd();

    // 绘制底面 
    texture.bindTex(2);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z - l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y - h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z - l);

    glEnd();

    // 绘制顶面 
    texture.bindTex(3);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z - l);

    glEnd();

    // 绘制左面 
    texture.bindTex(4);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z - l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z - l);

    glEnd();

    // 绘制右面 
    texture.bindTex(5);

    glBegin(GL_QUADS);

    // 指定纹理坐标和顶点坐标 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z - l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y - h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glEnd();

    glPopMatrix();                 // 绘制结束 
    glDisable(GL_TEXTURE_2D);
    if (lp)                         // 恢复光照状态 
        glEnable(GL_LIGHT0);
}