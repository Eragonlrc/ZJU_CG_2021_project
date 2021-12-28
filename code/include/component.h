#pragma once
#include <gl/glut.h>
#include <math.h>

class Robot
{
private:
	double head_r, tran_x, tran_y, tran_z;
	int type;
	double  body_l, body_w, body_h;
	double  arm_mul, leg_mul;
	;
public:
	Robot(int type = 0);
	void draw();
	void drawHead1();
	void drawHead2();
	void drawBody1();
	void drawBody2();
	void drawArm1();
	void drawArm2();
	void drawLeg1();
	void drawLeg2();
	void setTranslate(double x,double y,double z);
	void setType(int t);
	int getType();
};
