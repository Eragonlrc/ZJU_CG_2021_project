#pragma once
#include "component.h"
#include "belt.h"
#include "map.h"
#include <gl/glut.h>
#include <vector>
#include <math.h>

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
	bool showAttachment;
	int clockWise;
	Robot* robot;
public:
	Arm(int dx, int dy, int f, int t);
	void update();
	void DrawFoundation();
	void DrawJoint1();
	void DrawArm1();
	void DrawArm2();
	void DrawArm3();
	void DrawClaw();
	void Draw();
	void Attach(Robot* rbt);
	void activate();
	int getDirection();
	Robot* getAttached();
	int getState();
	void updateItem();
};
