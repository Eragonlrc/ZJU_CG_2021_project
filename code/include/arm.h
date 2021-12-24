#pragma once
#include "component.h"
#include <gl/glut.h>
#include <vector>
#include <math.h>

class Arm {
private:
	static float arm1[];
	static float arm2[];
	static float arm3[];
	static float clawAngle;
	static int state;
	static int phase;
	static int from;
	static int to;
	static int x;
	static int y;
	static int tx;
	static int ty;
	static bool showAttachment;
	static int clockWise;
	Robot* robot;
public:
	Arm();
	static void init(int dx, int dy, int f, int t);
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
	Robot* getAttached();
	int getState();
};
