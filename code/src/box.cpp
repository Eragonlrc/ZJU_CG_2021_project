#include"box.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

TexLoader Box::t(2);

void Box::init()
{
	x = 0.5; y = 0.5; z = 0.5; 
	tran_y = 8; tran_z = 8; tran_y2 = 0;
	t.genTex();
	t.loadTex(0, "textures/iron.bmp");
	t.loadTex(1, "textures/belt.bmp");
	addRobot();
	setType(1);
}

Box::Box() = default;

void Box::deleteRobot()
{
	has_r = 0;
}

void Box::setRType(int r_typ)
{
	r.setType(r_typ);
}

int Box::getRType()
{
	return r.getType();
}

void Box::setType(int b_typ)
{
	typ = b_typ;
	if (typ == 1)
	{
		for (int i = 0; i <= 5; i++)
			need[i] = 9;
		has_r = 1;
	}
	else
	{
		srand((unsigned)time(NULL));
		need[0] = rand()%2+1;
		need[1] = rand()%2+3;
		need[2] = rand()%2+5;
		need[3] = need[2];
		need[4] = rand()%2+7;
		need[5] = need[4];
		if (need[5] == 8) cneedt = 5;
		else cneedt = 6;
		has_r = 0;
	}
}

void Box::addRobot()
{
	has_r = 1;
}

void Box::receiveRobot()
{
	for (int i = 0; i < 6; i++)
	{
		if (need[i] == r.getType())
		{
			cneedt--;
			need[i] = 0;
			break;
		}//感觉要加点互动 
	}
	if (cneedt == 0) printf("you are so good!!!!");   //组装好了！！！！！
}

void Box::draw()
{
	r.setTranslate(tran_x, tran_y + y + tran_y2, tran_z);

	glEnable(GL_TEXTURE_2D);
	t.bindTex(0);

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScalef(x, y, z);
	glBegin(GL_QUADS);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, -1);

	glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, -1, 1);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	t.bindTex(1);

	glPushMatrix();
	glTranslatef(tran_x, tran_y+tran_y2, tran_z);
	glScalef(x, y, z);
	glBegin(GL_QUADS);

	glTexCoord2f(1, 1);  glVertex3f(-0.8, 1, 0.8);
	glTexCoord2f(1, 0);  glVertex3f(-0.8, 1, -0.8);
	glTexCoord2f(0, 0);  glVertex3f(0.8, 1, -0.8);
	glTexCoord2f(0, 1);  glVertex3f(0.8, 1, 0.8);

	glEnd();
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);

	if (typ == 1)
	{
		if (has_r == 1)
		{
			r.draw();
			if (tran_y2 < 0) tran_y2 += 0.01;
		}
		else
		{
			if (tran_y2 < -y * 1.4)
			{
				addRobot();
				srand((unsigned)time(NULL));
				setRType(rand() % 8 + 1);
			}
			else tran_y2 -= 0.01;
		}
	}//起点

	if (typ == 2)
	{
		drawWholeRobot();
		if (has_r == 1)
		{
			r.draw();
			if (tran_y2 < -y * 1.4)
			{
				deleteRobot();
				receiveRobot();
			}
			else tran_y2 -= 0.01;
		}
		else
		{
			if (tran_y2 < 0) tran_y2 += 0.01;
		}
	}
}

void Box::drawWholeRobot()  //还需调整位置，发现部件的比例不对 
{
	Robot rr;

	rr.setType(need[0]);
	rr.setTranslate(tran_x -0.8, tran_y+0.2, tran_z);
	rr.draw();

	rr.setType(need[1]);
	rr.setTranslate(tran_x - 0.8, tran_y, tran_z);
	rr.draw();

	rr.setType(need[2]);
	rr.setTranslate(tran_x - 1.2, tran_y, tran_z);
	rr.draw();

	rr.setType(need[3]);
	rr.setTranslate(tran_x + 0.4, tran_y, tran_z);
	rr.draw();

	rr.setType(need[4]);
	rr.setTranslate(tran_x - 0.8, tran_y-0.2, tran_z);
	rr.draw();

	if (need[5] != 8)
	{
		rr.setType(need[5]);
		rr.setTranslate(tran_x - 0.8, tran_y-0.2, tran_z);
		rr.draw();
	}
	
}
