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
	ObjLoader(string filename);//��ȡ����
	ObjLoader();
	void Draw();//���ƺ���
	double scale;
	double tran_x, tran_y, tran_z;
	void Setscale(double s);
	void Settran(double tx,double ty,double tz);
private:
	vector<vector<GLfloat>> v;//��Ŷ���(x,y,z)����
	vector<vector<GLint>> f;//������������������
	vector<vector<GLfloat>> vn;
	vector<vector<GLint>> fn;
};

//������ʹ��
//ObjLoader obj("body1.obj");
//obj.Draw();
