#pragma once
#include <gl/glut.h>
#include <vector>
#include "texture.h"
#include "map.h"

class Belt{
private:
    static const float beltSpeed;
    static const float beltHeight;
    static const float radius[];
    static const float radius0;
    static const float height[];
    static const float deltaAngle;
    static const float endCut;
private:
    static TexLoader texture;
    static float beltMove;
    typedef std::vector<Point> PointSet;
    typedef std::vector<Point>::iterator PointInterator;
    PointSet points;
    void drawStraight(bool start = 0, bool end = 0);
    void drawCornerCW();
    void drawCornerCCW();
    void drawEnd();
    void drawSingle();
public:
    Belt();
    Belt(PointInterator begin, PointInterator end);
    ~Belt();

    static void init();
    static void update();

    void pushPoint(int z, int x);
    void updateMap(int begin = 0, int end = -1); // update points of index [begin, end) in map
    Belt* delPoint(int index);      // delete a point, return a new Belt object if one forms
    void merge(Belt *belt);         // merge belt after current object, destroy belt

    void draw();
    void print();
};

