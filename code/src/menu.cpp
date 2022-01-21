#include "menu.h"

static const float defaultColor[] = { 1, 1, 1 };
static const float lineColor[] = { 0, 1, 0 };

extern float menuWidthRatio = 0.2;
extern Editor editor;

Button::Button(float _x, float _y, float _w, float _h) :
	texture(1), selected(0), x(_x), y(_y), w(_w), h(_h) {}

void Button::init(const char* texPath) {
	texture.genTex();
	texture.loadTex(0, texPath);
}

void Button::draw() {
	// draw content
	glEnable(GL_TEXTURE_2D);
	texture.bindTex(0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(x - w / 2, y - h / 2);
	glTexCoord2f(1, 0); glVertex2f(x + w / 2, y - h / 2);
	glTexCoord2f(1, 1); glVertex2f(x + w / 2, y + h / 2);
	glTexCoord2f(0, 1); glVertex2f(x - w / 2, y + h / 2);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// if selected, draw line frame
	if (selected) {
		glColor3fv(lineColor);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x - w / 2, y - h / 2);
		glVertex2f(x + w / 2, y - h / 2);
		glVertex2f(x + w / 2, y + h / 2);
		glVertex2f(x - w / 2, y + h / 2);
		glEnd();
		glColor3fv(defaultColor);
	}
}

bool Button::click(float mouseX, float mouseY) {
	return mouseX >= x - w / 2 && mouseX < x + w / 2 && mouseY > y - h / 2 && mouseY < y + h / 2;
}

void Button::changeStatus() {
	selected = !selected;
}

Menu::Menu() : texture(1) {};
void Menu::init() {
	texture.genTex();
	texture.loadTex(0, "textures/menu/menu.bmp");
	buttons.push_back(Button(0, 0.8, 1.2 * whratio, 0.15));
	buttons[0].init("textures/menu/buttonBelt.bmp");
	buttons.push_back(Button(0, 0.6, 1.2 * whratio, 0.15));
	buttons[1].init("textures/menu/buttonArm.bmp");
	buttons.push_back(Button(0, 0.4, 1.2 * whratio, 0.15));
	buttons[2].init("textures/menu/buttonDelete.bmp");
	buttons[editor.getMode()].changeStatus();
}

void Menu::setWH(float _whratio) {
	whratio = _whratio;
}

void Menu::setView(float wWidth, float wHeight) {
	float ratio = (float)wWidth / wHeight;
	glViewport(0, 0, wWidth * menuWidthRatio, wHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-whratio, whratio, -1, 1);
	glMatrixMode(GL_MODELVIEW);
}

void Menu::draw() {
	// menu background
	glEnable(GL_TEXTURE_2D);
	texture.bindTex(0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(-whratio, -1);
	glTexCoord2f(1, 0); glVertex2f(whratio, -1);
	glTexCoord2f(1, 1); glVertex2f(whratio, 1);
	glTexCoord2f(0, 1); glVertex2f(-whratio, 1);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// buttons
	for (int i = 0; i < buttons.size(); i++)
		buttons[i].draw();
}

void Menu::click(float mouseX, float mouseY) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].click(mouseX, mouseY)) {	// click hit
			if (i != editor.getMode()) {					// if hit button is not current status, change status
				buttons[editor.getMode()].changeStatus();
				buttons[i].changeStatus();
				editor.changeMode(i);
			}
			break;
		}
	}
}
