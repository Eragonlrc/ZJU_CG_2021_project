#pragma once
#include <gl/glut.h>
#include "texture.h"

class Belt{
private:
    static TexLoader texture;
    float beltMove;
    static const float beltSpeed;
    static const float beltHeight;
    static const float radius[];
    static const float radius0;
    static const float height[];
    void draw0();
    void draw1();
public:
    Belt();
    static void init();
    void draw(int id);
};

