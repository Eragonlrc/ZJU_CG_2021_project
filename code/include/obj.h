#pragma once
#include <vector>
#include <iostream>
#include <gl/glut.h>

using namespace std;

class ObjLoader
{
public:
	struct vertex
	{
		float x;
		float y;
		float z;
	};
	ObjLoader(string filename);//读取函数
	ObjLoader();
	void Draw();//绘制函数
	double scale;
	double tran_x, tran_y, tran_z;
	void Setscale(double s);
	void Settran(double tx,double ty,double tz);
private:
	vector<vector<GLfloat>> v;//存放顶点(x,y,z)坐标
	vector<vector<GLint>> f;//存放面的三个顶点索引
	vector<vector<GLfloat>> vn;
	vector<vector<GLint>> fn;
};

//定义与使用
//ObjLoader obj("body1.obj");
//obj.Draw();
