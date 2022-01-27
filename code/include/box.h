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
	double tran_y2;   //�������λ��
	static TexLoader t;
	int has_r,r_typ;  //�Ƿ����㲿��
	double rotate,frotate; //���Ƴ�Ʒ��ת �Լ�������ת
	Robot* r;  //r�ǻ��ڸ����ϵ����
	Robot* rneed;  //rneed��need�����
	bool ready, rec_flag;
	int typ; //typ=1Ϊ��㣬typ=2Ϊ�յ㣬����box.settype(1��2)�ı������յ㣬Ĭ��Ϊ��� 

	int need[6],cneedt; //�յ���Ҫ�Ĳ�������
	int rec[6], crect; //�Ѿ����յĲ�������
public:
	Box(float _x, float _y, float _z);
	void draw();
	void setRType(int r_typ);  //����robot��type
	int getRType();   //��ȡ����㲿������
	void setType(int b_typ); //����Ϊ�����յ�
	void setNeed(int i, int r_typ);
	void drawWholeRobot();
	Robot* getAttached();   //��㱻������
	void attach(Robot *robot);
	void receiveRobot();
	void addRobot();
	void deleteRobot();
	bool getReady();
	void drawright();
	void drawwrong();
};

