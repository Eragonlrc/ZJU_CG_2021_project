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
private:
    static TexLoader texture;
    static float beltMove;
    std::vector<std::pair<int, int> > points;
    void drawStraight(bool rev);
    void drawCornerCW(bool rev);
    void drawCornerCCW(bool rev);
public:
    Belt();
    static void init();
    static void update();
    void pushPoint(int z, int x);
    void draw();
};

