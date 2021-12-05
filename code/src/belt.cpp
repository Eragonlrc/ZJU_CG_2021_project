#include "belt.h"
#include <math.h>
static const float C = acos(-1) / 180;

TexLoader Belt::texture(2);
const float Belt::beltHeight = 0.05;
const float Belt::beltSpeed = 0.002;
// const float Belt::normal[][3] = { {1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1} };

Belt::Belt(): beltMove(0){}

void Belt::init() {
    texture.genTex();
    texture.texLoad(0, "texture/iron.bmp");
    texture.texLoad(1, "texture/belt.bmp");
}

void Belt::draw(int id) {
    switch (id){
        case 0: draw0(); break;
        case 1: draw1(); break;
    }
}

void Belt::draw0(){
    static const float frame2f[][2] = {{0.1, 0.2}, {0.9, 0.2}, {0.9, 0.4}, {0.8, 0.4},
                                {0.8, 0.3}, {0.2, 0.3}, {0.2, 0.4}, {0.1, 0.4},
                                {0.2, 0.2}, {0.8, 0.2}};
    static const int seq[][4] = {{0, 8, 6, 7}, {8, 9, 4, 5}, {9, 1, 2, 3}};

    glEnable(GL_TEXTURE_2D);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // front
    // glNormal3fv(normal[4]);
    for(int i = 0; i < 3; i++){
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(frame2f[seq[i][j]]);
            glVertex3f(frame2f[seq[i][j]][0], frame2f[seq[i][j]][1], 1);
        }
    }
    // back
    // glNormal3fv(normal[5]);
    for (int i = 2; i >= 0; i--) {
        for (int j = 3; j >= 0; j--) {
            glTexCoord2fv(frame2f[seq[i][j]]);
            glVertex3f(frame2f[seq[i][j]][0], frame2f[seq[i][j]][1], 0);
        }
    }
    // side
    for(int i = 0; i < 7; i++){
        switch(i){
            case 0:{
                // glNormal3fv(normal[3]);
                glTexCoord2f(frame2f[i+1][0], 1); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 1);
                glTexCoord2f(frame2f[i][0], 1); glVertex3f(frame2f[i][0], frame2f[i][1], 1);
                glTexCoord2f(frame2f[i][0], 0); glVertex3f(frame2f[i][0], frame2f[i][1], 0);
                glTexCoord2f(frame2f[i+1][0], 0); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 0);
                break;
            }
            case 1: case 5:{
                // glNormal3fv(normal[0]);
                glTexCoord2f(frame2f[i+1][1], 1); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 1);
                glTexCoord2f(frame2f[i][1], 1); glVertex3f(frame2f[i][0], frame2f[i][1], 1);
                glTexCoord2f(frame2f[i][1], 0); glVertex3f(frame2f[i][0], frame2f[i][1], 0);
                glTexCoord2f(frame2f[i+1][1], 0); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 0);
            }
            case 3:{
                // glNormal3fv(normal[1]);
                glTexCoord2f(frame2f[i+1][1], 1); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 1);
                glTexCoord2f(frame2f[i][1], 1); glVertex3f(frame2f[i][0], frame2f[i][1], 1);
                glTexCoord2f(frame2f[i][1], 0); glVertex3f(frame2f[i][0], frame2f[i][1], 0);
                glTexCoord2f(frame2f[i+1][1], 0); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 0);
            }
            default:{
                // glNormal3fv(normal[2]);
                glTexCoord2f(frame2f[i+1][0], 1); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 1);
                glTexCoord2f(frame2f[i][0], 1); glVertex3f(frame2f[i][0], frame2f[i][1], 1);
                glTexCoord2f(frame2f[i][0], 0); glVertex3f(frame2f[i][0], frame2f[i][1], 0);
                glTexCoord2f(frame2f[i+1][0], 0); glVertex3f(frame2f[i+1][0], frame2f[i+1][1], 0);
                break;
            }
        }
    }
    // glNormal3fv(normal[1]);
    glTexCoord2f(frame2f[0][1], 1); glVertex3f(frame2f[0][0], frame2f[0][1], 1);
    glTexCoord2f(frame2f[7][1], 1); glVertex3f(frame2f[7][0], frame2f[7][1], 1);
    glTexCoord2f(frame2f[7][1], 0); glVertex3f(frame2f[7][0], frame2f[7][1], 0);
    glTexCoord2f(frame2f[0][1], 0); glVertex3f(frame2f[0][0], frame2f[0][1], 0);
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    // glNormal3fv(normal[1]);
    // part 1
    glTexCoord2f(frame2f[5][0], 1 - beltMove); glVertex3f(frame2f[5][0], frame2f[5][1] + beltHeight, 1);
    glTexCoord2f(frame2f[4][0], 1 - beltMove); glVertex3f(frame2f[4][0], frame2f[4][1] + beltHeight, 1);
    glTexCoord2f(frame2f[4][0], 0); glVertex3f(frame2f[4][0], frame2f[4][1] + beltHeight, beltMove);
    glTexCoord2f(frame2f[5][0], 0); glVertex3f(frame2f[5][0], frame2f[5][1] + beltHeight, beltMove);
    // part 2
    glTexCoord2f(frame2f[5][0], 1); glVertex3f(frame2f[5][0], frame2f[5][1] + beltHeight, beltMove);
    glTexCoord2f(frame2f[4][0], 1); glVertex3f(frame2f[4][0], frame2f[4][1] + beltHeight, beltMove);
    glTexCoord2f(frame2f[4][0], 1 - beltMove); glVertex3f(frame2f[4][0], frame2f[4][1] + beltHeight, 0);
    glTexCoord2f(frame2f[5][0], 1 - beltMove); glVertex3f(frame2f[5][0], frame2f[5][1] + beltHeight, 0);

    glEnd();
    glDisable(GL_TEXTURE_2D);

    beltMove += beltSpeed;
    if (beltMove >= 1) beltMove -= 1;
}

void Belt::draw1() {
    static const float radium[] = { 0.1, 0.2, 0.8, 0.9 };
    static const float height[] = { 0.2, 0.3, 0.4 };
    static const float deltaAngle = 1;
    static const float beltAngleSpeed = beltSpeed / radium[1] / C;

    glEnable(GL_TEXTURE_2D);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    // glNormal3fv(normal[3]);
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float seg[][2] = { {radium[0] * cos(rad[0]), radium[0] * sin(rad[0])},
                           {radium[3] * cos(rad[0]), radium[3] * sin(rad[0])},
                           {radium[3] * cos(rad[1]), radium[3] * sin(rad[1])},
                           {radium[0] * cos(rad[1]), radium[0] * sin(rad[1])} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(seg[j]);
            glVertex3f(seg[j][1], height[0], seg[j][0]);
        }
    }
    // up
    //glNormal3fv(normal[2]);
    for(int k = 0; k < 3; k++){
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radium[k] * cos(rad[0]), radium[k] * sin(rad[0])},
                               {radium[k+1] * cos(rad[0]), radium[k+1] * sin(rad[0])},
                               {radium[k+1] * cos(rad[1]), radium[k+1] * sin(rad[1])},
                               {radium[k] * cos(rad[1]), radium[k] * sin(rad[1])} };
            for (int j = 0; j < 4; j++) {
                glTexCoord2fv(seg[j]);
                glVertex3f(seg[j][1], height[k==1 ? 1 : 2], seg[j][0]);
            }
        }
    }
    // side
    for (int k = 0; k < 4; k++) {
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radium[k] * cos(rad[0]), radium[k] * sin(rad[0])},
                               {radium[k] * cos(rad[1]), radium[k] * sin(rad[1])} };
            glTexCoord2f(radium[k] * rad[0] - int(radium[k] * rad[0]), height[2]); glVertex3f(seg[0][1], height[2], seg[0][0]);
            glTexCoord2f(radium[k] * rad[1] - int(radium[k] * rad[1]), height[2]); glVertex3f(seg[1][1], height[2], seg[1][0]);
            if (k == 0 || k == 3) {
                glTexCoord2f(radium[k] * rad[1] - int(radium[k] * rad[1]), height[0]); glVertex3f(seg[1][1], height[0], seg[1][0]);
                glTexCoord2f(radium[k] * rad[0] - int(radium[k] * rad[0]), height[0]); glVertex3f(seg[0][1], height[0], seg[0][0]);
            }
            else {
                glTexCoord2f(radium[k] * rad[1] - int(radium[k] * rad[1]), height[1]); glVertex3f(seg[1][1], height[1], seg[1][0]);
                glTexCoord2f(radium[k] * rad[0] - int(radium[k] * rad[0]), height[1]); glVertex3f(seg[0][1], height[1], seg[0][0]);
            }
        }
    }
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    // // part 1
    // for (float i = beltMove; i < 90; i += deltaAngle) {
    //     float rad[] = { i * C, (i + deltaAngle) * C };
    //     float texrad[] = { (i - beltMove) * C, (i + deltaAngle - beltMove) * C };
    //     float seg[][2] = { {radium[1] * cos(rad[0]), radium[1] * sin(rad[0])},
    //                        {radium[2] * cos(rad[0]), radium[2] * sin(rad[0])},
    //                        {radium[2] * cos(rad[1]), radium[2] * sin(rad[1])},
    //                        {radium[1] * cos(rad[1]), radium[1] * sin(rad[1])} };
    //     float texseg[][2] = { {radium[1] * cos(texrad[0]), radium[1] * sin(texrad[0])},
    //                           {radium[2] * cos(texrad[0]), radium[2] * sin(texrad[0])},
    //                           {radium[2] * cos(texrad[1]), radium[2] * sin(texrad[1])},
    //                           {radium[1] * cos(texrad[1]), radium[1] * sin(texrad[1])} };
    //     for (int j = 0; j < 4; j++) {
    //         glTexCoord2fv(texseg[j]);
    //         glVertex3f(seg[j][1], height[1] + beltHeight, seg[j][0]);
    //     }
    // }
    // // part 2
    // for (float i = 0; i < beltMove; i += deltaAngle) {
    //     float rad[] = { i * C, (i + deltaAngle) * C };
    //     float texrad[] = { (i + (90 - beltMove)) * C, (i + deltaAngle + (90 - beltMove)) * C };
    //     float seg[][2] = { {radium[1] * cos(rad[0]), radium[1] * sin(rad[0])},
    //                        {radium[2] * cos(rad[0]), radium[2] * sin(rad[0])},
    //                        {radium[2] * cos(rad[1]), radium[2] * sin(rad[1])},
    //                        {radium[1] * cos(rad[1]), radium[1] * sin(rad[1])} };
    //     float texseg[][2] = { {radium[1] * cos(texrad[0]), radium[1] * sin(texrad[0])},
    //                           {radium[2] * cos(texrad[0]), radium[2] * sin(texrad[0])},
    //                           {radium[2] * cos(texrad[1]), radium[2] * sin(texrad[1])},
    //                           {radium[1] * cos(texrad[1]), radium[1] * sin(texrad[1])} };
    //     for (int j = 0; j < 4; j++) {
    //         glTexCoord2fv(texseg[j]);
    //         glVertex3f(seg[j][1], height[1] + beltHeight, seg[j][0]);
    //     }
    // }
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float texrad[2];
        if (i >= beltMove) texrad[0] = (i - beltMove) * C, texrad[1] = (i + deltaAngle - beltMove) * C;
        else texrad[0] = (i + (90 - beltMove)) * C, texrad[1] = (i + deltaAngle + (90 - beltMove)) * C;
        float seg[][2] = { {radium[1] * cos(rad[0]), radium[1] * sin(rad[0])},
                           {radium[2] * cos(rad[0]), radium[2] * sin(rad[0])},
                           {radium[2] * cos(rad[1]), radium[2] * sin(rad[1])},
                           {radium[1] * cos(rad[1]), radium[1] * sin(rad[1])} };
        float texseg[][2] = { {radium[1], radium[1] * texrad[0] - int(radium[1] * texrad[0])},
                              {radium[2], radium[2] * texrad[0] - int(radium[2] * texrad[0])},
                              {radium[2], radium[2] * texrad[1] - int(radium[2] * texrad[1])},
                              {radium[1], radium[1] * texrad[1] - int(radium[1] * texrad[1])} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texseg[j]);
            glVertex3f(seg[j][1], height[1] + beltHeight, seg[j][0]);
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    beltMove += beltAngleSpeed;
    if (beltMove >= 90) beltMove -= 90;
}