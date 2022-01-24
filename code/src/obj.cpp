#include"obj.h"
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
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
			istringstream s(line.substr(2));
			s >> x; s >> y; s >> z;
			Point.push_back(x);
			Point.push_back(y);
			Point.push_back(z);
			v.push_back(Point);

		}
		else if (line.substr(0, 1) == "f")
		{
			vector<GLint> vIndexSets;
			GLint u, v, w;
			istringstream vtns(line.substr(2));
			vtns >> u; vtns >> v; vtns >> w;
			vIndexSets.push_back(u - 1);
			vIndexSets.push_back(v - 1);
			vIndexSets.push_back(w - 1);
			f.push_back(vIndexSets);
		}
	}
	file.close();
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

			glNormal3f(VN[0], VN[1], VN[2]);//绘制法向量

			glVertex3f(a.x+512, a.y+0.5, a.z+512);//绘制面
			glVertex3f(b.x+512, b.y+0.5, b.z+512);
			glVertex3f(c.x+512, c.y+0.5, c.z+512);
			if (f[i].size() == 4) glVertex3f(d.x+512,d.y+0.5,d.z+512);
		
	}
	glEnd();
}
