#pragma once
#include"gl/glut.h"
#include"texture.h"
#include"component.h"

//画box调用box.draw()
//如果box是起点，那么只需要在夹住起点的零件时，调用box.delete_robot()即可
//如果box是终点，当一个零件被放到表面时，需要调用box.add_robot()和box.setrtype(robot.gettype()) robot即为放上去的零件 

class Box
{
private:
	double x, y, z;    //长宽高
	double tran_x, tran_y, tran_z;     //box位移
	double tran_y2;   //顶隔板的位移
	static TexLoader t;
	int has_r,r_typ;  //是否有零部件
	double rotate,frotate; //控制成品旋转 以及符号旋转
	Robot* r;  //r是画在隔板上的零件
	Robot* rneed;  //rneed是need的零件
	bool ready, rec_flag;
	int typ; //typ=1为起点，typ=2为终点，调用box.settype(1或2)改变起点或终点，默认为起点 

	int need[6],cneedt; //终点需要的部件类型
	int rec[6], crect; //已经接收的部件类型
public:
	Box(float _x, float _y, float _z);
	void draw();
	void setRType(int r_typ);  //设置robot的type
	int getRType();   //获取起点零部件类型
	void setType(int b_typ); //设置为起点或终点
	void setNeed(int i, int r_typ);
	void drawWholeRobot();
	Robot* getAttached();   //起点被人拿走
	void attach(Robot *robot);
	void receiveRobot();
	void addRobot();
	void deleteRobot();
	bool getReady();
	void drawright();
	void drawwrong();
};

