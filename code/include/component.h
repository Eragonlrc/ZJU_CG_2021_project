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
	Robot(int type);
	void draw();
	void drawhead1();
	void drawhead2();
	void drawbody1();
	void drawbody2();
	void drawarm1();
	void drawarm2();
	void drawleg1();
	void drawleg2();
	void settranslate(double x,double y,double z);
	void settype(int t);
	int gettype();
};
