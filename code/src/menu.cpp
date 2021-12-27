#include "menu.h"

static const float defaultColor[] = { 1, 1, 1 };
static const float lineColor[] = { 0, 1, 0 };

Button::Button(const char* texPath, float _x, float _y, float _w, float _h) :
	texture(1), selected(0), x(_x), y(_y), w(_w), h(_h) {
	texture.genTex();
	texture.loadTex(0, texPath);
}

void Button::draw() {
	// draw content
	glEnable(GL_TEXTURE_2D);
	texture.bindTex(0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x, y);
	glTexCoord2f(1, 0); glVertex2f(x + w, y);
	glTexCoord2f(1, 1); glVertex2f(x + w, y + h);
	glTexCoord2f(0, 1); glVertex2f(x, y + h);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// if selected, draw line frame
	if (selected) {
		glColor3fv(lineColor);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x, y);
		glVertex2f(x + w, y);
		glVertex2f(x + w, y + h);
		glVertex2f(x, y + h);
		glEnd();
		glColor3fv(defaultColor);
	}
}

bool Button::click(float mouseX, float mouseY) {
	return mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h;
}

void Button::changeStatus() {
	selected = !selected;
}

Menu::Menu(float whratio) : x(0), y(0), w(whratio), h(1){
	buttons.push_back(Button("textures/button/buttonBelt.bmp", 0.2 * whratio, 0.2, 0.2 * whratio, 0.2));
}

void Menu::draw() {

}

int Menu::click(float mouseX, float mouseY) {
	return 0;
}

int Menu::getStatus() {
	return 0;
}