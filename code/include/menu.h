#pragma once
#include "texture.h"
#include <vector>

class Button {
private:
	bool selected;
	float x, y, w, h;
	TexLoader texture;
public:
	Button(const char *texPath, float _x, float _y, float _w, float _h);
	void draw();
	bool click(float mouseX, float mouseY);
	void changeStatus();
};

class Menu {
private:
	float x, y, w, h;
	std::vector<Button> buttons;
public:
	Menu(float whratio);
	void draw();
	int click(float mouseX, float mouseY);
	int getStatus();
};