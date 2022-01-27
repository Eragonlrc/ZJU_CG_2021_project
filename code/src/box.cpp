#include "box.h"
#include "map.h"
#include "arm.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

extern Map map;

TexLoader Box::t(2);

Box::Box(float _x, float _y, float _z)
{
	x = 0.2; y = 0.15; z = 0.2; 
	tran_x = _x;  tran_y = _y; tran_z = _z; 
	tran_y2 = 0; rotate = 0; frotate = 360;
	r_typ = 1; rec_flag = false;
	t.genTex();
	t.loadTex(0, "textures/iron.bmp");
	t.loadTex(1, "textures/belt.bmp");
	rneed = new Robot(1);
	r = new Robot(1);
	setType(2);
}

bool Box::getReady()
{
	return ready;
}

void Box::setRType(int rr_typ)
{
	r->setType(rr_typ);
	r_typ = rr_typ;
}

int Box::getRType()
{
	return r->getType();
}

Robot* Box::getAttached()
{
	has_r = 0;
	return r;
}

void Box::addRobot()  //起点自己产生零件
{
	r = new Robot(r_typ);
	has_r = 1;
}

void Box::attach(Robot* robot)
{
	r = robot;
	has_r = 1;
	r_typ = r->getType();
}

void Box::deleteRobot()
{
	has_r = 0;
}

void Box::setType(int b_typ)
{
	typ = b_typ;
	if (typ == 1)  //起点
	{
		for (int i = 0; i <= 5; i++)
			need[i] = 9;
		addRobot();
	}
	else
	{
		for (int i = 0; i < 6; i++)
			rec[i] = 0;
		crect = 0;
		if (need[5] == 8) cneedt = 5;
		else cneedt = 6;
		deleteRobot();
	}
}

void Box::setNeed(int i, int r_typ) {
	need[i] = r_typ;
}

void Box::receiveRobot()
{
	rec_flag = false;
	for (int i = 0; i < 6; i++)
	{
		if (need[i] == r->getType() && rec[i] == 0) 
		{
			rec[i] = need[i];
			crect++;
			rec_flag = true;
			break;
		}//感觉要加点互动 
	}
	deleteRobot();
	frotate = 0;
}

void Box::drawright()
{
	glColor3f(0, 1, 0);
	glPushMatrix();
	glTranslatef(tran_x,tran_y+0.1,tran_z);
	glRotatef(frotate, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.2,0.2,0);
	glVertex3f(-0.1,0.3,0);
	glVertex3f(0,0.2,0);
	glVertex3f(0,0,0);

	glVertex3f(0,0,0);
	glVertex3f(0.4,0.4,0);
	glVertex3f(0.3,0.5,0);
	glVertex3f(0,0.2,0);
	glEnd();
	glPopMatrix();
}

void Box::drawwrong()
{
	glColor3f(1, 0, 0);
	glPushMatrix();
	glTranslatef(tran_x, tran_y + 0.3, tran_z);
	glRotatef(frotate, 0, 1, 0);
	glBegin(GL_QUADS);
	glVertex3f(-0.2, 0.1, 0);
	glVertex3f(-0.1, 0.2, 0);
	glVertex3f(0.2, -0.1, 0);
	glVertex3f(0.1, -0.2, 0);

	glVertex3f(-0.2, -0.1, 0);
	glVertex3f(-0.1, -0.2, 0);
	glVertex3f(0.2, 0.1, 0);
	glVertex3f(0.1, 0.2, 0);
	glEnd();
	glPopMatrix();
}


void Box::draw()
{
	ready = (typ == 1 && has_r && tran_y2 >= 0) || (typ == 2 && !has_r && tran_y2 >= 0);
	rotate += 0.1;
	if (rotate > 360) rotate -= 360;
	frotate += 6;
	if (frotate > 720) frotate -= 360;

	glEnable(GL_TEXTURE_2D);
	t.bindTex(0);

	glPushMatrix();
	glTranslatef(tran_x, tran_y, tran_z);
	glScalef(x, y, z);
	
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, 1);	// 
	glTexCoord2f(1, 0); glVertex3f(-1, -1, 1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, 1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1); glVertex3f(-1, 1, -1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, -1);

	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(1, 1, 1);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(1, 1); glVertex3f(-1, -1, 1);
	glTexCoord2f(1, 0); glVertex3f(-1, -1, -1);
	glTexCoord2f(0, 0); glVertex3f(-1, 1, -1);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 1);

	glNormal3f(0, -1, 0);
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

	glNormal3f(0, 1, 0);
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
			glPushMatrix();
			glTranslatef(tran_x, tran_y +y +tran_y2,tran_z);
			r->draw();
			glPopMatrix();
			if (tran_y2 < 0) tran_y2 += 0.002;
			else {
				static const int mapOffset[][2] = { {-1, 0}, {0, -1}, {1, 0}, {0, 1} };
				for (int i = 0; i < 4; i++) {
					Map::MapUnit neighbor = map.getMap(tran_z + mapOffset[i][0], tran_x + mapOffset[i][1]);
					if (neighbor.type == MAP_ARM) {
						Arm* arm = (Arm*)(neighbor.obj);
						if (arm->getState() == 0 && arm->getDirection() == i) {
							arm->attach(r);
							arm->activate();
							has_r = 0;
							break;
						}
					}
				}
			}
		}
		else
		{
			if (tran_y2 < -y * 1.4)
			{
				addRobot();
			}
			else tran_y2 -= 0.002;
		}
	}//起点

	if (typ == 2)
	{
		glPushMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		drawWholeRobot();
		glPopMatrix();
		if (has_r == 1)
		{
			glPushMatrix();
			glTranslatef(tran_x, tran_y + y + tran_y2, tran_z);
			r->draw();
			glPopMatrix();
			if (tran_y2 < -y * 1.4)
			{
				receiveRobot();
			}
			else tran_y2 -= 0.002;
		}
		else
		{
			if (tran_y2 < 0) tran_y2 += 0.002;
		}
	}

	if (frotate < 360)
		if (rec_flag) drawright();
		else drawwrong();

	/*if (cneedt == crect)
	{
		glPushMatrix();
		glTranslated(0, -0.2, 0);
		drawWholeRobot();
		glPopMatrix();
	}*/
}

void Box::drawWholeRobot()  
{
	glColor3d(1.0,1.0,1.0);
	rneed->setType(need[0]);
	glPushMatrix();
	glTranslatef(tran_x , tran_y+0.8, tran_z);
	glRotated(rotate, 0, 1, 0);
	if (need[0] == rec[0]) {	// 该部件已得到
		glColor3f(0.0, 1.0, 0.0);
		rneed->draw();
		glColor3f(1.0, 1.0, 1.0);
	}
	else 
		rneed->draw();
	
	glPopMatrix();

	rneed->setType(need[1]);
	glPushMatrix();
	glTranslatef(tran_x , tran_y+0.62, tran_z);
	glRotated(rotate, 0, 1, 0);
	if (need[1] == rec[1]) {	// 该部件已得到
		glColor3f(0.0, 1.0, 0.0);
		rneed->draw();
		glColor3f(1.0, 1.0, 1.0);
	}
	else
		rneed->draw();
	glPopMatrix();

	rneed->setType(need[2]);
	glPushMatrix();
	glTranslatef(tran_x, tran_y+0.7, tran_z);
	glRotated(rotate, 0, 1, 0);
	glTranslatef(0.15, 0, 0);
	if (need[2] == rec[2]) {	// 该部件已得到
		glColor3f(0.0, 1.0, 0.0);
		rneed->draw();
		glColor3f(1.0, 1.0, 1.0);
	}
	else
		rneed->draw();
	glPopMatrix();

	rneed->setType(need[3]);
	glPushMatrix();
    glTranslatef(tran_x, tran_y+0.7, tran_z);
	glRotated(rotate, 0, 1, 0);
	glTranslatef(-0.15, 0, 0);
	if (need[3] == rec[3]) {	// 该部件已得到
		glColor3f(0.0, 1.0, 0.0);
		rneed->draw();
		glColor3f(1.0, 1.0, 1.0);
	}
	else
		rneed->draw();
	glPopMatrix();

	if (need[4] != 8)
	{
		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x, tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		glTranslatef(0.08, 0, 0);
		if (need[4] == rec[4]) {	// 该部件已得到
			glColor3f(0.0, 1.0, 0.0);
			rneed->draw();
			glColor3f(1.0, 1.0, 1.0);
		}
		else
			rneed->draw();
		glPopMatrix();

		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x , tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		glTranslatef(-0.08, 0, 0);
		if (need[5] == rec[5]) {	// 该部件已得到
			glColor3f(0.0, 1.0, 0.0);
			rneed->draw();
			glColor3f(1.0, 1.0, 1.0);
		}
		else
			rneed->draw();
		glPopMatrix();
	}
	else
	{
		rneed->setType(need[4]);
		glPushMatrix();
		glTranslatef(tran_x, tran_y + 0.45, tran_z);
		glRotated(rotate, 0, 1, 0);
		if (need[4] == rec[4]) {	// 该部件已得到
			glColor3f(0.0, 1.0, 0.0);
			rneed->draw();
			glColor3f(1.0, 1.0, 1.0);
		}
		else
			rneed->draw();
		glPopMatrix();
	}
	
}
