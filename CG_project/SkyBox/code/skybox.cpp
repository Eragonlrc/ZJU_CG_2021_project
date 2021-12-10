#include "SkyBox.h"

TexLoader CSkyBox::texture(6);

CSkyBox::CSkyBox(){
}

CSkyBox::~CSkyBox(){
}

// ��պг�ʼ�� 
bool CSkyBox::Init(){
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
void  CSkyBox::CreateSkyBox(float x, float y, float z, float box_width, float box_height, float box_length){
    const unsigned int MAP_WIDTH = 1024;
    const unsigned int CELL_WIDTH = 16;
    const unsigned int MAP = MAP_WIDTH * CELL_WIDTH / 2;
    // ��ó����й���״̬ 
    GLboolean lp;
    glGetBooleanv(GL_LIGHTING, &lp);

    // ������պг� �� �� 
    float width = MAP * box_width / 8;
    float height = MAP * box_height / 8;
    float length = MAP * box_length / 8;

    // ������պ�����λ�� 
    x = x + MAP / 8 - width / 2;
    y = y + MAP / 24 - height / 2;
    z = z + MAP / 8 - length / 2;

    glDisable(GL_LIGHTING);            // �رչ��� 

    // ��ʼ���� 
    glPushMatrix();
    glTranslatef(-x, -y, -z);

    // ���Ʊ��� 
    texture.bindTex(0);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);

    glEnd();

    // ����ǰ�� 
    texture.bindTex(1);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);

    glEnd();

    // ���Ƶ��� 
    texture.bindTex(2);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);

    glEnd();

    // ���ƶ��� 
    texture.bindTex(3);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);

    glEnd();

    // �������� 
    texture.bindTex(4);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);

    glEnd();

    // �������� 
    texture.bindTex(5);

    glBegin(GL_QUADS);

    // ָ����������Ͷ������� 
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();

    glPopMatrix();                 // ���ƽ��� 

    if (lp)                         // �ָ�����״̬ 
        glEnable(GL_LIGHTING);

}