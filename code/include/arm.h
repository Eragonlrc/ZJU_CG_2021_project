#pragma once
#include "component.h"
#include "belt.h"
#include "map.h"
#include "box.h"
#include <gl/glut.h>
#include <vector>
#include <math.h>

#define ARM_COLOR_DEFAULT 0
#define ARM_COLOR_DRAWING 1
#define ARM_COLOR_WARNING 2

class Arm {
private:
	float arm1[2];
	float arm2[2];
	float arm3[2];
	float clawAngle;
	int state;
	int phase;
	int from;
	int to;
	int x;
	int y;
	int tx;
	int ty;
	int color;
	bool showAttachment;
	int clockWise;
	Robot* robot;
public:
	Arm(int dx, int dy, int f, int t);
	void update();
	void drawFoundation();
	void drawJoint1();
	void drawArm1();
	void drawArm2();
	void drawArm3();
	void drawClaw();
	void draw();
	void attach(Robot* rbt);
	void activate();
	Robot* getAttached();
	Point getPosition();
	int getDirection();
	int getState();
	void updateItem();
	void setColor(int c);
	void setFrom(int f);
	void setTo(int t);
};
