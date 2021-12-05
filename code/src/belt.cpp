#include "belt.h"
#include <math.h>
static const float C = acos(-1) / 180;

TexLoader Belt::texture(2);
const float Belt::beltHeight = 0.05;
const float Belt::beltSpeed = 0.002;
const float Belt::radius[] = { 0.1, 0.2, 0.8, 0.9 };
const float Belt::radius0 = 2 / acos(-1);
const float Belt::height[] = { 0.2, 0.3, 0.4 };
float Belt::beltMove = 0;

Belt::Belt() = default;

void Belt::init() {
    texture.genTex();
    texture.texLoad(0, "texture/iron.bmp");
    texture.texLoad(1, "texture/belt.bmp");
}

void Belt::update() {
    beltMove += beltSpeed;
    if (beltMove >= 1) beltMove -= 1;
}

void Belt::pushPoint(int z, int x) {
    points.push_back(std::pair<int, int>(z, x));
}

void Belt::draw() {
    for (int i = 1; i < points.size() - 1; i++) {
        bool rev = 0;
        int from, to;
        glPushMatrix();
        if (points[i].first == points[i - 1].first) {
            if (points[i].second > points[i - 1].second) from = 0;
            else from = 2;
        }
        else {
            if (points[i].first > points[i - 1].first) from = 1;
            else from = 3;
        }
        if (points[i].first == points[i + 1].first) {
            if (points[i].second > points[i + 1].second) to = 0;
            else to = 2;
        }
        else {
            if (points[i].first > points[i + 1].first) to = 1;
            else to = 3;
        }
        glTranslatef(points[i].second, 0, points[i].first);
        switch ((to - from + 4) % 4) {
            case 3: case 1: {
                glRotatef(-90 * from, 0, 1, 0);
                if((to - from + 4) % 4 == 3) drawCornerCCW(0);
                else drawCornerCW(0);
                break;
            }
            case 2: {
                glRotatef(-90 * from, 0, 1, 0);
                drawStraight(0);
                break;
            }
        }
        glPopMatrix();
    }
}

void Belt::drawStraight(bool rev) {
    float _beltMove = rev ? 1 - beltMove : beltMove;
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    glTexCoord2f(radius[0], 0); glVertex3f(0, height[0], radius[0]);
    glTexCoord2f(radius[3], 0); glVertex3f(0, height[0], radius[3]);
    glTexCoord2f(radius[3], 1); glVertex3f(1, height[0], radius[3]);
    glTexCoord2f(radius[0], 1); glVertex3f(1, height[0], radius[0]);
    // up
    for (int i = 0; i < 3; i++) {
        glTexCoord2f(radius[i], 0); glVertex3f(0, height[i == 1 ? 1 : 2], radius[i]);
        glTexCoord2f(radius[i+1], 0); glVertex3f(0, height[i == 1 ? 1 : 2], radius[i+1]);
        glTexCoord2f(radius[i+1], 1); glVertex3f(1, height[i == 1 ? 1 : 2], radius[i+1]);
        glTexCoord2f(radius[i], 1); glVertex3f(1, height[i == 1 ? 1 : 2], radius[i]);
    }
    // side
    for (int i = 0; i < 4; i++) {
        glTexCoord2f(height[2], 0); glVertex3f(0, height[2], radius[i]);
        glTexCoord2f(height[2], 1); glVertex3f(1, height[2], radius[i]);
        if (i == 0 || i == 3) {
            glTexCoord2f(height[0], 1); glVertex3f(1, height[0], radius[i]);
            glTexCoord2f(height[0], 0); glVertex3f(0, height[0], radius[i]);
        }
        else {
            glTexCoord2f(height[1], 1); glVertex3f(1, height[1], radius[i]);
            glTexCoord2f(height[1], 0); glVertex3f(0, height[1], radius[i]);
        }
    }
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    // part 1
    glTexCoord2f(radius[2], 1 - _beltMove); glVertex3f(1, height[1] + beltHeight, radius[2]);
    glTexCoord2f(radius[1], 1 - _beltMove); glVertex3f(1, height[1] + beltHeight, radius[1]);
    glTexCoord2f(radius[1], 0); glVertex3f(_beltMove, height[1] + beltHeight, radius[1]);
    glTexCoord2f(radius[2], 0); glVertex3f(_beltMove, height[1] + beltHeight, radius[2]);
    // part 2
    glTexCoord2f(radius[2], 1); glVertex3f(_beltMove, height[1] + beltHeight, radius[2]);
    glTexCoord2f(radius[1], 1); glVertex3f(_beltMove, height[1] + beltHeight, radius[1]);
    glTexCoord2f(radius[1], 1 - _beltMove); glVertex3f(0, height[1] + beltHeight, radius[1]);
    glTexCoord2f(radius[2], 1 - _beltMove); glVertex3f(0, height[1] + beltHeight, radius[2]);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Belt::drawCornerCW(bool rev) {
    static const float deltaAngle = 0.1;
    float _beltMove = rev ? (1 - beltMove) / radius0 / C : beltMove / radius0 / C;

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float seg[][2] = { {radius[0] * cos(rad[0]), radius[0] * sin(rad[0])},
                           {radius[3] * cos(rad[0]), radius[3] * sin(rad[0])},
                           {radius[3] * cos(rad[1]), radius[3] * sin(rad[1])},
                           {radius[0] * cos(rad[1]), radius[0] * sin(rad[1])} };
        float texseg[][2] = { {radius[0], radius0 * rad[0]},
                              {radius[3], radius0 * rad[0]},
                              {radius[3], radius0 * rad[1]},
                              {radius[0], radius0 * rad[1]} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texseg[j]);
            glVertex3f(seg[j][1], height[0], seg[j][0]);
        }
    }
    // up
    for(int k = 0; k < 3; k++){
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radius[k] * cos(rad[0]), radius[k] * sin(rad[0])},
                               {radius[k+1] * cos(rad[0]), radius[k+1] * sin(rad[0])},
                               {radius[k+1] * cos(rad[1]), radius[k+1] * sin(rad[1])},
                               {radius[k] * cos(rad[1]), radius[k] * sin(rad[1])} };
            float texseg[][2] = { {radius[k], radius0 * rad[0]},
                                  {radius[k+1], radius0 * rad[0]},
                                  {radius[k+1], radius0 * rad[1]},
                                  {radius[k], radius0 * rad[1]} };
            for (int j = 0; j < 4; j++) {
                glTexCoord2fv(texseg[j]);
                glVertex3f(seg[j][1], height[k==1 ? 1 : 2], seg[j][0]);
            }
        }
    }
    // side
    for (int k = 0; k < 4; k++) {
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radius[k] * cos(rad[0]), radius[k] * sin(rad[0])},
                               {radius[k] * cos(rad[1]), radius[k] * sin(rad[1])} };
            glTexCoord2f(height[2], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[2], seg[0][0]);
            glTexCoord2f(height[2], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[2], seg[1][0]);
            if (k == 0 || k == 3) {
                glTexCoord2f(height[0], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[0], seg[1][0]);
                glTexCoord2f(height[0], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[0], seg[0][0]);
            }
            else {
                glTexCoord2f(height[1], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[1], seg[1][0]);
                glTexCoord2f(height[1], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[1], seg[0][0]);
            }
        }
    }
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float texrad[2];
        if (i >= _beltMove) texrad[0] = (i - _beltMove) * C, texrad[1] = (i + deltaAngle - _beltMove) * C;
        else texrad[0] = (i + (90 - _beltMove)) * C, texrad[1] = (i + deltaAngle + (90 - _beltMove)) * C;
        float seg[][2] = { {radius[1] * cos(rad[0]), radius[1] * sin(rad[0])},
                           {radius[2] * cos(rad[0]), radius[2] * sin(rad[0])},
                           {radius[2] * cos(rad[1]), radius[2] * sin(rad[1])},
                           {radius[1] * cos(rad[1]), radius[1] * sin(rad[1])} };
        float texseg[][2] = { {radius[1], radius0 * texrad[0]},
                              {radius[2], radius0 * texrad[0]},
                              {radius[2], radius0 * texrad[1]},
                              {radius[1], radius0 * texrad[1]} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texseg[j]);
            glVertex3f(seg[j][1], height[1] + beltHeight, seg[j][0]);
        }
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Belt::drawCornerCCW(bool rev) {
    static const float deltaAngle = 0.1;
    float _beltMove = rev ? (1 - beltMove) / radius0 / C : beltMove / radius0 / C;

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float seg[][2] = { {radius[0] * cos(rad[0]), radius[0] * sin(rad[0])},
                           {radius[3] * cos(rad[0]), radius[3] * sin(rad[0])},
                           {radius[3] * cos(rad[1]), radius[3] * sin(rad[1])},
                           {radius[0] * cos(rad[1]), radius[0] * sin(rad[1])} };
        float texseg[][2] = { {radius[0], radius0 * rad[0]},
                              {radius[3], radius0 * rad[0]},
                              {radius[3], radius0 * rad[1]},
                              {radius[0], radius0 * rad[1]} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texseg[j]);
            glVertex3f(seg[j][1], height[0], 1 - seg[j][0]);
        }
    }
    // up
    for(int k = 0; k < 3; k++){
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radius[k] * cos(rad[0]), radius[k] * sin(rad[0])},
                               {radius[k+1] * cos(rad[0]), radius[k+1] * sin(rad[0])},
                               {radius[k+1] * cos(rad[1]), radius[k+1] * sin(rad[1])},
                               {radius[k] * cos(rad[1]), radius[k] * sin(rad[1])} };
            float texseg[][2] = { {radius[k], radius0 * rad[0]},
                                  {radius[k+1], radius0 * rad[0]},
                                  {radius[k+1], radius0 * rad[1]},
                                  {radius[k], radius0 * rad[1]} };
            for (int j = 0; j < 4; j++) {
                glTexCoord2fv(texseg[j]);
                glVertex3f(seg[j][1], height[k==1 ? 1 : 2], 1 - seg[j][0]);
            }
        }
    }
    // side
    for (int k = 0; k < 4; k++) {
        for (float i = 0; i < 90; i += deltaAngle) {
            float rad[] = { i * C, (i + deltaAngle) * C };
            float seg[][2] = { {radius[k] * cos(rad[0]), radius[k] * sin(rad[0])},
                               {radius[k] * cos(rad[1]), radius[k] * sin(rad[1])} };
            glTexCoord2f(height[2], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[2], 1 - seg[0][0]);
            glTexCoord2f(height[2], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[2], 1 - seg[1][0]);
            if (k == 0 || k == 3) {
                glTexCoord2f(height[0], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[0], 1 - seg[1][0]);
                glTexCoord2f(height[0], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[0], 1 - seg[0][0]);
            }
            else {
                glTexCoord2f(height[1], radius[k] * rad[1] - int(radius[k] * rad[1])); glVertex3f(seg[1][1], height[1], 1 - seg[1][0]);
                glTexCoord2f(height[1], radius[k] * rad[0] - int(radius[k] * rad[0])); glVertex3f(seg[0][1], height[1], 1 - seg[0][0]);
            }
        }
    }
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    for (float i = 0; i < 90; i += deltaAngle) {
        float rad[] = { i * C, (i + deltaAngle) * C };
        float texrad[2];
        if (i >= _beltMove) texrad[0] = (i - _beltMove) * C, texrad[1] = (i + deltaAngle - _beltMove) * C;
        else texrad[0] = (i + (90 - _beltMove)) * C, texrad[1] = (i + deltaAngle + (90 - _beltMove)) * C;
        float seg[][2] = { {radius[1] * cos(rad[0]), radius[1] * sin(rad[0])},
                           {radius[2] * cos(rad[0]), radius[2] * sin(rad[0])},
                           {radius[2] * cos(rad[1]), radius[2] * sin(rad[1])},
                           {radius[1] * cos(rad[1]), radius[1] * sin(rad[1])} };
        float texseg[][2] = { {radius[2], radius0 * texrad[0]},
                              {radius[1], radius0 * texrad[0]},
                              {radius[1], radius0 * texrad[1]},
                              {radius[2], radius0 * texrad[1]} };
        for (int j = 0; j < 4; j++) {
            glTexCoord2fv(texseg[j]);
            glVertex3f(seg[j][1], height[1] + beltHeight, 1 - seg[j][0]);
        }
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}