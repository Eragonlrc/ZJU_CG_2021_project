#include "belt.h"
#include "texture.h"
#include "map.h"
#include <math.h>
static const float C = acos(-1) / 180;

extern Map map;

TexLoader Belt::texture(2);
const float Belt::beltHeight = 0.05;
const float Belt::beltSpeed = 0.002;
const float Belt::radius[] = { 0.1, 0.2, 0.8, 0.9 };
const float Belt::radius0 = 2 / acos(-1);
const float Belt::height[] = { 0.2, 0.3, 0.4 };
float Belt::beltMove = 0;
const float Belt::deltaAngle = 0.1;
const float Belt::endCut = 0.05;

Belt::Belt() = default;
Belt::Belt(PointInterator begin, PointInterator end) : points(begin, end) {}
Belt::~Belt() {
    for (int i = 0; i < points.size(); i++) { // delete belt from map
        map.write(points[0].first, points[0].second, MAP_BLANK, NULL, i);
    }
}

void Belt::print() {
    printf("%d\n", points.size());
    for (int i = 0; i < points.size(); i++) {
        printf("(%d, %d)\n", points[i].first, points[i].second);
    }
}

void Belt::init() {
    texture.genTex();
    texture.loadTex(0, "textures/iron.bmp");
    texture.loadTex(1, "textures/belt.bmp");
}

void Belt::update() {
    beltMove += beltSpeed;
    if (beltMove >= 1) beltMove -= 1;
}

void Belt::pushPoint(int z, int x) {
    points.push_back(Point(z, x));
}

void Belt::updateMap(int begin, int end) {
    if (end == -1) end = points.size();
    printf("%d\n", points.size());
    if (points.size() == 1) { // single belt
        map.write(points[0].first, points[0].second, MAP_BELT_SINGLE, this, 0);
        return;
    }
    for (int i = begin; i < end; i++) {
        int from, to;
        // check belt type
        if (i != 0) {
            if (points[i].first == points[i - 1].first) {
                if (points[i].second > points[i - 1].second) from = 0;
                else from = 2;
            }
            else {
                if (points[i].first > points[i - 1].first) from = 1;
                else from = 3;
            }
        }
        if (i != points.size() - 1) {
            if (points[i].first == points[i + 1].first) {
                if (points[i].second > points[i + 1].second) to = 0;
                else to = 2;
            }
            else {
                if (points[i].first > points[i + 1].first) to = 1;
                else to = 3;
            }
        }
        if (i == 0) {
            if (points[0].first == points[points.size() - 1].first && points[0].second == points[points.size() - 1].second) { // loop, from depend on the second last belt
                if (points[i].first == points[points.size() - 2].first) {
                    if (points[i].second > points[points.size() - 2].second) from = 0;
                    else from = 2;
                }
                else {
                    if (points[i].first > points[points.size() - 2].first) from = 1;
                    else from = 3;
                }
            }
            else from = (to + 2) % 4; // no loop, straight belt
        }
        else if (i == points.size() - 1) {
            if (points[0].first == points[i].first && points[0].second == points[i].second) break; // loop, skip the last belt
            else to = (from + 2) % 4; // no loop, straight belt
        }
        // update map
        switch ((to - from + 4) % 4) {
            case 1: {   // corner CW
                map.write(points[i].first, points[i].second, MAP_BELT_CORNER0 + from, this, i);
                break;
            }
            case 3: {   // corner CCW
                map.write(points[i].first, points[i].second, MAP_BELT_CORNER0 + to, this, i);
                break;
            }
            case 2: {   // straight
                if(from % 2 == 0) map.write(points[i].first, points[i].second, MAP_BELT_STRAIGHTX, this, i);
                else map.write(points[i].first, points[i].second, MAP_BELT_STRAIGHTZ, this, i);
                break;
            }
        }
    }
}

void Belt::draw() {
    bool loop = points[0].first == points[points.size() - 1].first && points[0].second == points[points.size() - 1].second;
    if (points.size() == 1) { // single belt
        glPushMatrix();
        glTranslatef(points[0].second, 0, points[0].first);
        drawSingle();
        glPopMatrix();
        return;
    }
    for (int i = 0; i < points.size(); i++) {
        int from, to;
        // check belt type
        if (i != 0) {
            if (points[i].first == points[i - 1].first) {
                if (points[i].second > points[i - 1].second) from = 0;
                else from = 2;
            }
            else {
                if (points[i].first > points[i - 1].first) from = 1;
                else from = 3;
            }
        }
        if (i != points.size() - 1) {
            if (points[i].first == points[i + 1].first) {
                if (points[i].second > points[i + 1].second) to = 0;
                else to = 2;
            }
            else {
                if (points[i].first > points[i + 1].first) to = 1;
                else to = 3;
            }
        }
        if (i == 0) {
            if (loop) { // loop, from depends on the second last belt
                if (points[i].first == points[points.size() - 2].first) {
                    if (points[i].second > points[points.size() - 2].second) from = 0;
                    else from = 2;
                }
                else {
                    if (points[i].first > points[points.size() - 2].first) from = 1;
                    else from = 3;
                }
            }
            else from = (to + 2) % 4; // no loop, straight belt
        }
        else if (i == points.size() - 1) {
            if (loop) break; // loop, skip the last belt
            else to = (from + 2) % 4; // no loop, straight belt
        }
        // draw belt
        glPushMatrix();
        glTranslatef(points[i].second, 0, points[i].first);
        glPushMatrix();
        switch ((to - from + 4) % 4) {
            case 3: case 1: {       // corner
                glRotatef(-90 * from, 0, 1, 0);
                if((to - from + 4) % 4 == 3) drawCornerCCW();
                else drawCornerCW();
                break;
            }
            case 2: {               // straight
                glRotatef(-90 * from, 0, 1, 0);
                drawStraight(!loop && (i == 0), !loop && (i == points.size() - 1));
                break;
            }
        }
        glPopMatrix();
        // if no loop, draw ending for first and last belt
        if (!loop) {
            if (i == 0) {
                glPushMatrix();
                glRotatef(-90 * from, 0, 1, 0);
                drawEnd();
                glPopMatrix();
            }
            else if (i == points.size() - 1) {
                glPushMatrix();
                glRotatef(-90 * to, 0, 1, 0);
                drawEnd();
                glPopMatrix();
            }
        }
        glPopMatrix();
    }
}

void Belt::drawStraight(bool start, bool end) {
    float _beltMove = beltMove;
    float startLen = 0, endLen = 1;
    if (start) startLen += endCut;
    if (end) endLen -= endCut;
    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    glTexCoord2f(radius[0], startLen); glVertex3f(startLen, height[0], radius[0]);
    glTexCoord2f(radius[3], startLen); glVertex3f(startLen, height[0], radius[3]);
    glTexCoord2f(radius[3], endLen); glVertex3f(endLen, height[0], radius[3]);
    glTexCoord2f(radius[0], endLen); glVertex3f(endLen, height[0], radius[0]);
    // up
    for (int i = 0; i < 3; i++) {
        glTexCoord2f(radius[i], startLen); glVertex3f(startLen, height[i == 1 ? 1 : 2], radius[i]);
        glTexCoord2f(radius[i+1], startLen); glVertex3f(startLen, height[i == 1 ? 1 : 2], radius[i+1]);
        glTexCoord2f(radius[i+1], endLen); glVertex3f(endLen, height[i == 1 ? 1 : 2], radius[i+1]);
        glTexCoord2f(radius[i], endLen); glVertex3f(endLen, height[i == 1 ? 1 : 2], radius[i]);
    }
    // side
    for (int i = 0; i < 4; i++) {
        glTexCoord2f(height[2], startLen); glVertex3f(startLen, height[2], radius[i]);
        glTexCoord2f(height[2], endLen); glVertex3f(endLen, height[2], radius[i]);
        if (i == 0 || i == 3) {
            glTexCoord2f(height[0], endLen); glVertex3f(endLen, height[0], radius[i]);
            glTexCoord2f(height[0], startLen); glVertex3f(startLen, height[0], radius[i]);
        }
        else {
            glTexCoord2f(height[1], endLen); glVertex3f(endLen, height[1], radius[i]);
            glTexCoord2f(height[1], startLen); glVertex3f(startLen, height[1], radius[i]);
        }
    }
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    if(_beltMove < startLen){
        glTexCoord2f(radius[2], endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[2]);
        glTexCoord2f(radius[1], endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[1], startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[2], startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[2]);
    }
    else if(_beltMove < endLen){
        // part 1
        glTexCoord2f(radius[2], endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[2]);
        glTexCoord2f(radius[1], endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[1], 0); glVertex3f(_beltMove, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[2], 0); glVertex3f(_beltMove, height[1] + beltHeight, radius[2]);
        // part 2
        glTexCoord2f(radius[2], 1); glVertex3f(_beltMove, height[1] + beltHeight, radius[2]);
        glTexCoord2f(radius[1], 1); glVertex3f(_beltMove, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[1], 1 + startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[2], 1 + startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[2]);
    }
    else{
        glTexCoord2f(radius[2], 1 + endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[2]);
        glTexCoord2f(radius[1], 1 + endLen - _beltMove); glVertex3f(endLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[1], 1 + startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[1]);
        glTexCoord2f(radius[2], 1 + startLen - _beltMove); glVertex3f(startLen, height[1] + beltHeight, radius[2]);
    }
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Belt::drawCornerCW() {
    float _beltMove = beltMove / radius0 / C;

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

void Belt::drawCornerCCW() {
    float _beltMove = beltMove / radius0 / C;

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

void Belt::drawEnd() {
    float _beltMove = beltMove;

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);

    glTexCoord2f(height[0], radius[0]); glVertex3f(endCut, height[0], radius[0]);
    glTexCoord2f(height[0], radius[1]); glVertex3f(endCut, height[0], radius[1]);
    glTexCoord2f(height[2], radius[1]); glVertex3f(endCut, height[2], radius[1]);
    glTexCoord2f(height[2], radius[0]); glVertex3f(endCut, height[2], radius[0]);

    glTexCoord2f(height[0], radius[1]); glVertex3f(endCut, height[0], radius[1]);
    glTexCoord2f(height[0], radius[2]); glVertex3f(endCut, height[0], radius[2]);
    glTexCoord2f(height[1] + beltHeight, radius[2]); glVertex3f(endCut, height[1] + beltHeight, radius[2]);
    glTexCoord2f(height[1] + beltHeight, radius[1]); glVertex3f(endCut, height[1] + beltHeight, radius[1]);

    glTexCoord2f(height[0], radius[2]); glVertex3f(endCut, height[0], radius[2]);
    glTexCoord2f(height[0], radius[3]); glVertex3f(endCut, height[0], radius[3]);
    glTexCoord2f(height[2], radius[3]); glVertex3f(endCut, height[2], radius[3]);
    glTexCoord2f(height[2], radius[2]); glVertex3f(endCut, height[2], radius[2]);

    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void Belt::drawSingle() {
    static const float edge[] = {radius[2] / 3 - (radius[3] - radius[2]), radius[2] / 3};

    glEnable(GL_TEXTURE_2D);
    glPushMatrix();
    glTranslatef(-0.5, 0, -0.5);
    // frame
    texture.bindTex(0);
    glBegin(GL_QUADS);
    // down
    glTexCoord2f(0.5 - edge[1], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[1], 0.5 - edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[1], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 + edge[1]);
    glTexCoord2f(0.5 - edge[1], 0.5 + edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 + edge[1]);
    // up
    glTexCoord2f(0.5 - edge[1], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[0], 0.5 - edge[1]); glVertex3f(0.5 + edge[0], height[2], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[0], 0.5 - edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 - edge[0]);
    glTexCoord2f(0.5 - edge[1], 0.5 - edge[0]); glVertex3f(0.5 - edge[1], height[2], 0.5 - edge[0]);

    glTexCoord2f(0.5 + edge[0], 0.5 - edge[1]); glVertex3f(0.5 + edge[0], height[2], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[1], 0.5 - edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 - edge[1]);
    glTexCoord2f(0.5 + edge[1], 0.5 + edge[0]); glVertex3f(0.5 + edge[1], height[2], 0.5 + edge[0]);
    glTexCoord2f(0.5 + edge[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 + edge[0]);

    glTexCoord2f(0.5 - edge[0], 0.5 + edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 + edge[0]);
    glTexCoord2f(0.5 + edge[1], 0.5 + edge[0]); glVertex3f(0.5 + edge[1], height[2], 0.5 + edge[0]);
    glTexCoord2f(0.5 + edge[1], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 + edge[1]);
    glTexCoord2f(0.5 - edge[0], 0.5 + edge[1]); glVertex3f(0.5 - edge[0], height[2], 0.5 + edge[1]);

    glTexCoord2f(0.5 - edge[1], 0.5 - edge[0]); glVertex3f(0.5 - edge[1], height[2], 0.5 - edge[0]);
    glTexCoord2f(0.5 - edge[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 - edge[0]);
    glTexCoord2f(0.5 - edge[0], 0.5 + edge[1]); glVertex3f(0.5 - edge[0], height[2], 0.5 + edge[1]);
    glTexCoord2f(0.5 - edge[1], 0.5 + edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 + edge[1]);
    // middle
    glTexCoord2f(0.5 - edge[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(0.5 + edge[0], 0.5 - edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(0.5 + edge[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 + edge[0]);
    glTexCoord2f(0.5 - edge[0], 0.5 + edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 + edge[0]);
    // outside
    glTexCoord2f(height[0], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(height[0], 0.5 + edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 + edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 - edge[1]);

    glTexCoord2f(height[0], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 + edge[1]);
    glTexCoord2f(height[0], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 + edge[1]);

    glTexCoord2f(height[0], 0.5 - edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(height[0], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 + edge[1]);
    glTexCoord2f(height[2], 0.5 - edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 - edge[1]);

    glTexCoord2f(height[0], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(height[0], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[0], 0.5 - edge[1]);
    glTexCoord2f(height[2], 0.5 + edge[1]); glVertex3f(0.5 + edge[1], height[2], 0.5 - edge[1]);
    glTexCoord2f(height[2], 0.5 - edge[1]); glVertex3f(0.5 - edge[1], height[2], 0.5 - edge[1]);
    // inside
    glTexCoord2f(height[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(height[0], 0.5 + edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 + edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 - edge[0]);

    glTexCoord2f(height[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 + edge[0]);
    glTexCoord2f(height[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 + edge[0]);

    glTexCoord2f(height[0], 0.5 - edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(height[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 + edge[0]);
    glTexCoord2f(height[2], 0.5 - edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 - edge[0]);

    glTexCoord2f(height[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(height[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[1], 0.5 - edge[0]);
    glTexCoord2f(height[2], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[2], 0.5 - edge[0]);
    glTexCoord2f(height[2], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[2], 0.5 - edge[0]);
    glEnd();

    // belt
    texture.bindTex(1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.5 - edge[0], 0.5 - edge[0]); glVertex3f(0.5 - edge[0], height[1] + beltHeight, 0.5 - edge[0]);
    glTexCoord2f(0.5 + edge[0], 0.5 - edge[0]); glVertex3f(0.5 + edge[0], height[1] + beltHeight, 0.5 - edge[0]);
    glTexCoord2f(0.5 + edge[0], 0.5 + edge[0]); glVertex3f(0.5 + edge[0], height[1] + beltHeight, 0.5 + edge[0]);
    glTexCoord2f(0.5 - edge[0], 0.5 + edge[0]); glVertex3f(0.5 - edge[0], height[1] + beltHeight, 0.5 + edge[0]);
    glEnd();
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

Belt* Belt::delPoint(int index) {
    Belt* newBelt = NULL;
    for (int i = 0; i < points.size(); i++) { // delete belt from map
        map.write(points[0].first, points[0].second, MAP_BLANK, NULL, i);
    }
    if (points.size() == 1) return NULL;
    else if (points[0].first == points[points.size() - 1].first && points[0].second == points[points.size() - 1].second) { // loop
        if (index == 0 || index == points.size() - 1) { // delete ending points
            points.pop_back();
            points.erase(points.begin());
        }
        else { // split in the middle, rearrange points
            points.pop_back();
            PointSet tmp(points.begin() + index + 1, points.end());
            points.erase(points.begin() + index, points.end());
            points.insert(points.begin(), tmp.begin(), tmp.end());
        }
    }
    else if (index == 0) points.erase(points.begin()); // delete first point
    else if (index == points.size() - 1) points.pop_back(); // delete last point
    else { // split into two belts
        newBelt = new Belt(points.begin() + index + 1, points.end());
        points.erase(points.begin() + index, points.end());
    }
    updateMap();
    if (newBelt) newBelt->updateMap();
    return newBelt;
}

void Belt::merge(Belt* belt) {
    int oriSize = points.size();
    points.insert(points.end(), belt->points.begin(), belt->points.end());
    belt->~Belt(); // destroy belt
    updateMap(oriSize); // update merged part in map
}