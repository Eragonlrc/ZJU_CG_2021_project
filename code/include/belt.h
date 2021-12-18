#pragma once
#include <gl/glut.h>
#include <vector>
#include "texture.h"
#include "map.h"
#include "component.h"

#define BELT_LIST_STRAIGHT listid
#define BELT_LIST_STRAIGHT_START (listid + 1)
#define BELT_LIST_STRAIGHT_END (listid + 2)
#define BELT_LIST_CORNERCW (listid + 3)
#define BELT_LIST_CORNERCCW (listid + 4)
#define BELT_LIST_END (listid + 5)
#define BELT_LIST_SINGLE (listid + 6)

class Belt{
public:
    typedef std::vector<Point> PointSet;
    typedef std::vector<Point>::iterator PointInterator;
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
    static GLuint listid;

    PointSet points;

    struct OnBeltComponent {
        Robot* component;
        float move;
        OnBeltComponent(Robot* _component, float _move) : component(_component), move(_move) {}
    };
    std::vector<OnBeltComponent> components;
private:
    static void drawStraightFrame(bool start = 0, bool end = 0);
    static void drawCornerCWFrame();
    static void drawCornerCCWFrame();
    static void drawEnd();
    static void drawSingle();
private:
    void getDirection(int i, int& from, int& to);

    void drawStraight(bool start = 0, bool end = 0);
    void drawCornerCW();
    void drawCornerCCW();
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
public:
    void updateComponents();
    void addComponent(Robot* component, int index = 0);
    void drawComponents();
};

