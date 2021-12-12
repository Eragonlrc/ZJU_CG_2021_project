#pragma once
#include <gl/glut.h>
#include <vector>
#include "texture.h"

class Belt{
private:
    static const float beltSpeed;
    static const float beltHeight;
    static const float radius[];
    static const float radius0;
    static const float height[];
    static const float deltaAngle;
private:
    static TexLoader texture;
    static float beltMove;
    std::vector<std::pair<int, int> > points;
    void drawStraight(bool rev = 0);
    void drawCornerCW(bool rev = 0);
    void drawCornerCCW(bool rev = 0);
    void drawEnd();
public:
    Belt();
    static void init();
    static void update();
    void pushPoint(int z, int x);
    void draw();
};

