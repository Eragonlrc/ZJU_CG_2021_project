#include "SkyBox.h"

TexLoader SkyBox::texture(6);

SkyBox::SkyBox(){
}

SkyBox::~SkyBox(){
}

// ��պг�ʼ�� 
bool SkyBox::init(){
    char filename[128]; // ���������ļ��� 
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