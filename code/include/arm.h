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

#define ARM_FOUNDATION_SPHERE armListId
#define ARM_JOINT1_SPHERE (armListId + 1)
#define ARM_JOINT2_SPHERE (armListId + 2)
#define ARM_CLAW_ROOT_SPHERE (armListId + 3)
#define ARM_CLAW_JOINT1_SPHERE (armListId + 4)
#define ARM_CLAW_END_SPHERE (armListId + 5)
#define ARM_1_CYLINDER (armListId + 6)
#define ARM_2_CYLINDER (armListId + 7)
#define ARM_3_CYLINDER (armListId + 8)
#define ARM_CLAW_1_CYLINDER (armListId + 9)
#define ARM_CLAW_2_CYLINDER (armListId + 10)
#define PI 3.14159

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
	GLuint armListId;
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
