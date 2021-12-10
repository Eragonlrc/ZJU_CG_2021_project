#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>

#define BITMAP_ID 0x4D42
#define MAXN 10		// ÿ��Loader���ͬʱ����10��ͼƬ
#define GL_CLAMP_TO_EDGE    0x812F

class TexLoader {
private:
	int texnum;
	unsigned int* texture;
	unsigned char* loadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
public:
	TexLoader(int n);
	void genTex();
	void loadTex(int i, const char* filename);
	void bindTex(int i);
};