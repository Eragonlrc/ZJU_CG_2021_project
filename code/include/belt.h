#pragma once
#include <gl/glut.h>
#include "texture.h"

class Belt{
private:
    static TexLoader texture;
    float beltMove;
    static const float beltSpeed;
    static const float normal[][3];
    static const float beltHeight;
    void draw0();
    void draw1();
public:
    Belt();
    static void init();
    void draw(int id);
};

