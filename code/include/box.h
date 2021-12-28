#pragma once
#include"gl/glut.h"
#include"texture.h"
#include"component.h"

//��box����box.draw()
//���box����㣬��ôֻ��Ҫ�ڼ�ס�������ʱ������box.delete_robot()����
//���box���յ㣬��һ��������ŵ�����ʱ����Ҫ����box.add_robot()��box.setrtype(robot.gettype()) robot��Ϊ����ȥ����� 

class Box
{
private:
	double x, y, z;    //�����
	double tran_x, tran_y, tran_z;     //boxλ��
	double tran_y2;
	static TexLoader t;
	int has_r;  //�Ƿ����㲿��
	Robot r;
	int typ; //typ=1Ϊ��㣬typ=2Ϊ�յ㣬����box.settype(1��2)�ı������յ㣬Ĭ��Ϊ��� 

	int need[6],cneedt; //�յ���Ҫ�Ĳ�������
public:
	Box();
	void init();
	void draw();
	void deleteRobot();
	void addRobot();
	void setRType(int r_typ);  //����robot��type
	int getRType();   //��ȡ����㲿������
	void setType(int b_typ); //����Ϊ�����յ�
	void receiveRobot();
	void drawWholeRobot();
};

