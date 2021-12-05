#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <gl/glut.h>

#define BITMAP_ID 0x4D42

class TexLoader {
private:
	int texnum;
	unsigned int* texture;
	unsigned char* loadBitmapFile(const char* filename, BITMAPINFOHEADER* bitmapInfoHeader);
public:
	TexLoader(int n);
	void genTex();
	void texLoad(int i, const char* filename);
	void bindTex(int i);
};