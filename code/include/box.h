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
	double tran_y2;
	static TexLoader t;
	int has_r;  //是否有零部件
	Robot r;
	int typ; //typ=1为起点，typ=2为终点，调用box.settype(1或2)改变起点或终点，默认为起点 

	int need[6],cneedt; //终点需要的部件类型
public:
	Box();
	void init();
	void draw();
	void delete_robot();
	void add_robot();
	void setrtype(int r_typ);  //设置robot的type
	int getrtype();   //获取起点零部件类型
	void settype(int b_typ); //设置为起点或终点
	void receive_robot();
	void drawwholerobot();
};

