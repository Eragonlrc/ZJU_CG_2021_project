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
	void Draw();//���ƺ���
private:
	vector<vector<GLfloat>> v;//��Ŷ���(x,y,z)����
	vector<vector<GLint>> f;//������������������
};

//������ʹ��
//ObjLoader obj("body1.obj");
//obj.Draw();
//��objLoaderֻ�ܴ���(v x y z)(f v1 v2 v3)�����
