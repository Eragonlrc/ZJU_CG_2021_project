#pragma once
#include <gl/glut.h>
#include <vector>
#include <math.h>

class Arm {
private:
	static float arm1[];
	static float arm2[];
	static float arm3[];
	static float clawAngle;
	static const float c;
	static const float L;
public:
	Arm();
	static void init();
	void update(int comp, int type, float deg);
	void DrawFoundation();
	void DrawJoint1();
	void DrawArm1();
	void DrawArm2();
	void DrawArm3();
	void DrawClaw();
	void Draw();
};
