#include"obj.h"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<cstdlib>

ObjLoader* pic[9];
GLuint objindex;

ObjLoader::ObjLoader()
{
}

ObjLoader::ObjLoader(string filename)
{
	ifstream file(filename);
	string line;
	while (getline(file, line))
	{
		if (line.substr(0, 1) == "v")
		{
			vector<GLfloat> Point;
			GLfloat x, y, z;

			int branch;
			if (line[1] == 't') continue;
			if (line[1] == 'n') branch = 3;
			else branch = 2;

			istringstream s(line.substr(branch));
			s >> x; s >> y; s >> z;
			Point.push_back(x);
			Point.push_back(y);
			Point.push_back(z);

			if (branch == 2) v.push_back(Point);
			else vn.push_back(Point);

		}
		else if (line.substr(0, 1) == "f")
		{
			vector<GLint> vIndexSets;
			string u, v, w, s;
			int l, r;

			istringstream vtns(line.substr(2));
			vtns >> u; vtns >> v; vtns >> w;

			l = u.find('/');
			s = u.substr(0, l);
			vIndexSets.push_back(stoi(s)-1);
			l = v.find('/');
			s = v.substr(0, l);
			vIndexSets.push_back(stoi(s)-1);
			l = w.find('/');
			s = w.substr(0, l);
			vIndexSets.push_back(stoi(s)-1);
			f.push_back(vIndexSets);//不知道要不要-1

			vIndexSets.clear();
			r = u.rfind('/');
			s = u.substr(r+1);
			vIndexSets.push_back(stoi(s)-1);
			r = v.rfind('/');
			s = v.substr(r+1);
			vIndexSets.push_back(stoi(s)-1);
			r = w.rfind('/');
			s = w.substr(r+1);
			vIndexSets.push_back(stoi(s)-1);
			fn.push_back(vIndexSets);
		}
	}
	file.close();

	scale = 1;
	tran_x = 0; tran_y = 0; tran_z = 0;

}

void ObjLoader::Draw()
{
	glBegin(GL_TRIANGLES);//开始绘制

	for (int i = 0; i < f.size(); i++) {
		GLfloat VN[3];//法线
		//三个顶点
		vertex a, b, c, normal, d;

			GLint firstVertexIndex = (f[i])[0];//取出顶点索引
			GLint secondVertexIndex = (f[i])[1];
			GLint thirdVertexIndex = (f[i])[2];

			a.x = (v[firstVertexIndex])[0];//第一个顶点
			a.y = (v[firstVertexIndex])[1];
			a.z = (v[firstVertexIndex])[2];

			b.x = (v[secondVertexIndex])[0]; //第二个顶点
			b.y = (v[secondVertexIndex])[1];
			b.z = (v[secondVertexIndex])[2];

			c.x = (v[thirdVertexIndex])[0]; //第三个顶点
			c.y = (v[thirdVertexIndex])[1];
			c.z = (v[thirdVertexIndex])[2];

			if (f[i].size() == 4) {
				GLint fourthVertexIndex = (f[i])[3];
				d.x = (v[fourthVertexIndex])[0]; //第四个顶点
				d.y = (v[fourthVertexIndex])[1];
				d.z = (v[fourthVertexIndex])[2];
			}

			GLfloat vec1[3], vec2[3], vec3[3];//计算法向量
			//(x2-x1,y2-y1,z2-z1)
			vec1[0] = a.x - b.x;
			vec1[1] = a.y - b.y;
			vec1[2] = a.z - b.z;

			//(x3-x2,y3-y2,z3-z2)
			vec2[0] = a.x - c.x;
			vec2[1] = a.y - c.y;
			vec2[2] = a.z - c.z;

			//(x3-x1,y3-y1,z3-z1)
			vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
			vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
			vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

			GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));

			VN[0] = vec3[0] / D;
			VN[1] = vec3[1] / D;
			VN[2] = vec3[2] / D;

			VN[0] = vn[fn[i][0]][0];
			VN[1] = vn[fn[i][0]][1];
			VN[2] = vn[fn[i][0]][2];
			glNormal3f(VN[0], VN[1], VN[2]);
			glVertex3f(a.x*scale+tran_x, a.y*scale+tran_y, a.z*scale+tran_z);//绘制面
			VN[0] = vn[fn[i][1]][0];
			VN[1] = vn[fn[i][1]][1];
			VN[2] = vn[fn[i][1]][2];
			glNormal3f(VN[0], VN[1], VN[2]);
			glVertex3f(b.x*scale+tran_x, b.y*scale+tran_y, b.z*scale+tran_z);
			VN[0] = vn[fn[i][2]][0];
			VN[1] = vn[fn[i][2]][1];
			VN[2] = vn[fn[i][2]][2];
			glNormal3f(VN[0], VN[1], VN[2]);
			glVertex3f(c.x*scale+tran_x, c.y*scale+tran_y, c.z*scale+tran_z);
			
	}
	glEnd();
}

void ObjLoader::Setscale(double s)
{
	scale = s;
}

void ObjLoader::Settran(double tx,double ty,double tz)
{
	tran_x = tx; tran_y = ty; tran_z = tz;
}

void Objinit()
{
	pic[1] = new ObjLoader("obj/head1.obj");
	pic[2] = new ObjLoader("obj/head2.obj");
	pic[3] = new ObjLoader("obj/body1.obj");
	pic[4] = new ObjLoader("obj/body2.obj");
	pic[5] = new ObjLoader("obj/arm1.obj");
	pic[6] = new ObjLoader("obj/arm2.obj");
	pic[7] = new ObjLoader("obj/leg1.obj");
	pic[8] = new ObjLoader("obj/leg2.obj");
	pic[1]->Setscale(0.025);
	pic[2]->Setscale(0.025);
	pic[3]->Setscale(0.3);
	pic[4]->Setscale(0.038);
	pic[5]->Setscale(0.04);
	pic[6]->Setscale(0.08);
	pic[7]->Setscale(0.03);
	pic[8]->Setscale(0.06);

	objindex = glGenLists(8);
	for (int i = 0; i < 8; i++)
	{
		glNewList(objindex+i,GL_COMPILE);
		pic[i + 1]->Draw();
		glEndList();
	}
}
