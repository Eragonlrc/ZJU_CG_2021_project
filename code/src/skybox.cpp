#include "SkyBox.h"

TexLoader SkyBox::texture(6);

SkyBox::SkyBox(){
}

SkyBox::~SkyBox(){
}

// ��պг�ʼ�� 
bool SkyBox::init(){
    char filename[128];                                         // ���������ļ��� 
    char bmpName[6][128] = { "back","front","bottom","top","left","right" };
    texture.genTex();
    for (int i = 0; i < 6; i++){
        sprintf_s(filename, "textures/box/");
        strcat_s(filename, bmpName[i]);
        strcat_s(filename, ".bmp");
        texture.loadTex(i, filename);
        // �����˲�: 
        /*
            ����GL_TEXTURE_WRAP_S��GL_TEXTURE_WRAP_Tͨ��������ΪGL_REPEAT��GL_CLAMP���ַ�ʽ��
            �������Ķ���δ��������ʱ��GL_REPEAT��ʾ����Ĳ������ظ��ķ�ʽ��䣻GL_CLAMP��ʾ����Ĳ�����������Ե������������䡣
            ��ʵ�ʻ����У�����һ�����GL_CLAMP_EDGE��������������˽ӷ촦��ϸ�ߣ���ǿ����պе���ʵ�С�*/
        
        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);*/
        // �������� 
        /*gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture[i].imageWidth,
            texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
            texture[i].image);*/
    }
    return true;

}

// ������պ� 
void  SkyBox::createSkyBox(float x, float y, float z, float box_width, float box_height, float box_length){
    // ��ó����й���״̬ 
    GLboolean lp;
    glGetBooleanv(GL_LIGHT0, &lp);

    // ������պг� �� �� 
    float width = BOX_SIZE * box_width;
    float height = BOX_SIZE * box_height;
    float length = BOX_SIZE * box_length;

    // �볤 ��� ���
    float w = width / 2;
    float h = height / 2;
    float l = length / 2;

    glDisable(GL_LIGHT0);            // �رչ��� 

    // ��ʼ���� 
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();

    // ���Ʊ��� 
    texture.bindTex(0);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y - h, z - l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - w, y + h, z - l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w, y - h, z - l);

    glEnd();

    // ����ǰ�� 
    texture.bindTex(1);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z + l);

    glEnd();

    // ���Ƶ��� 
    texture.bindTex(2);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z - l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y - h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z - l);

    glEnd();

    // ���ƶ��� 
    texture.bindTex(3);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z - l);

    glEnd();

    // �������� 
    texture.bindTex(4);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x - w, y + h, z - l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x - w, y + h, z + l);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x - w, y - h, z + l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x - w, y - h, z - l);

    glEnd();

    // �������� 
    texture.bindTex(5);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + w, y - h, z - l);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + w, y - h, z + l);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + w, y + h, z + l);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + w, y + h, z - l);
    glEnd();

    glPopMatrix();                 // ���ƽ��� 
    glDisable(GL_TEXTURE_2D);
    if (lp)                         // �ָ�����״̬ 
        glEnable(GL_LIGHT0);
}