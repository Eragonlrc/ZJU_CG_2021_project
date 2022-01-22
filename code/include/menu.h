#pragma once
#include "texture.h"
#include "edit.h"
#include <vector>

class Button {
private:
	bool selected;
	float x, y, w, h;
	TexLoader texture;
public:
	Button(float _x, float _y, float _w, float _h);
	void init(const char* texPath);
	void draw();
	bool click(float mouseX, float mouseY);
	void changeStatus();
};

class Menu {
private:
	TexLoader texture;
	std::vector<Button> buttons;
	float whratio;
public:
	Menu();
	void init();
	void setWH(float _whratio);
	void setView(float wWidth, float wHeight);
	void draw();
	void click(float mouseX, float mouseY);
};