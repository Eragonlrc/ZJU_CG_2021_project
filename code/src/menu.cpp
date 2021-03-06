#include "menu.h"

static const float defaultColor[] = { 1, 1, 1 };
static const float lineColor[] = { 0.2, 0.5, 0.2 };

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
		glLineWidth(3);
		glBegin(GL_LINE_LOOP);
		glVertex2f(x - w / 2, y - h / 2);
		glVertex2f(x + w / 2, y - h / 2);
		glVertex2f(x + w / 2, y + h / 2);
		glVertex2f(x - w / 2, y + h / 2);
		glEnd();
		glColor3fv(defaultColor);
		glLineWidth(1);
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
	// mode buttons
	buttons.push_back(Button(0, 0.8, 1.2 * whratio, 0.2));
	buttons[0].init("textures/menu/buttonBelt.bmp");
	buttons.push_back(Button(0, 0.55, 1.2 * whratio, 0.2));
	buttons[1].init("textures/menu/buttonArm.bmp");
	buttons.push_back(Button(0, 0.3, 1.2 * whratio, 0.2));
	buttons[2].init("textures/menu/buttonDelete.bmp");
	buttons.push_back(Button(0, 0.05, 1.2 * whratio, 0.2));
	buttons[3].init("textures/menu/buttonLightSource.bmp");
	buttons[editor.getMode()].changeStatus();
	// light color buttons
	buttons.push_back(Button(-0.18, -0.2, 0.1, 0.1));
	buttons[4].init("textures/menu/buttonWhite.bmp");
	buttons.push_back(Button(-0.06, -0.2, 0.1, 0.1));
	buttons[5].init("textures/menu/buttonRed.bmp");
	buttons.push_back(Button(0.06, -0.2, 0.1, 0.1));
	buttons[6].init("textures/menu/buttonGreen.bmp");
	buttons.push_back(Button(0.18, -0.2, 0.1, 0.1));
	buttons[7].init("textures/menu/buttonBlue.bmp");
	buttons.push_back(Button(-0.12, -0.325, 0.1, 0.1));
	buttons[8].init("textures/menu/buttonYellow.bmp");
	buttons.push_back(Button(0, -0.325, 0.1, 0.1));
	buttons[9].init("textures/menu/buttonMagenta.bmp");
	buttons.push_back(Button(0.12, -0.325, 0.1, 0.1));
	buttons[10].init("textures/menu/buttonCyan.bmp");
	buttons[editor.getLightColor() + EDITOR_MODE_NUM].changeStatus();
	// luminance buttons
	buttons.push_back(Button(-0.18, -0.45, 0.1, 0.1));
	buttons[11].init("textures/menu/buttonMinus.bmp");
	buttons.push_back(Button(0.18, -0.45, 0.1, 0.1));
	buttons[12].init("textures/menu/buttonPlus.bmp");
}

void Menu::setWH(float _whratio) {
	whratio = _whratio;
}

void Menu::setView(float wWidth, float wHeight) {
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
	// luminance bar
	float luminance = editor.getLightLuminance();
	glBegin(GL_QUADS);
	glColor3f(0, 0, 0);
	glVertex2f(-0.1, -0.4); glVertex2f(-0.1, -0.5);
	glColor3f(1, 1, 1);
	glVertex2f(0.1, -0.5); glVertex2f(0.1, -0.4);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.2 * luminance - 0.1, -0.4);
	glVertex2f(0.2 * luminance - 0.09, -0.39);
	glVertex2f(0.2 * luminance - 0.11, -0.39);
	glVertex2f(0.2 * luminance - 0.1, -0.5);
	glVertex2f(0.2 * luminance - 0.09, -0.51);
	glVertex2f(0.2 * luminance - 0.11, -0.51);
	glEnd();
	glBegin(GL_QUADS);
	glVertex2f(0.2 * luminance - 0.101, -0.4); glVertex2f(0.2 * luminance - 0.101, -0.5);
	glVertex2f(0.2 * luminance - 0.099, -0.5); glVertex2f(0.2 * luminance - 0.099, -0.4);
	glEnd();
	glColor3f(1, 1, 1);
}

void Menu::click(float mouseX, float mouseY) {
	for (int i = 0; i < buttons.size(); i++) {
		if (buttons[i].click(mouseX, mouseY)) {	// click hit
			if (i <= EDITOR_MODE_LIGHTSOURCE) { // mode button
				if (i != editor.getMode()) { // if hit button is not current editor mode, change mode
					buttons[editor.getMode()].changeStatus();
					buttons[i].changeStatus();
					editor.changeMode(i);
				}
			}
			else if (i < EDITOR_MODE_NUM + LIGHT_COLOR_NUM) { // light color button
				int color = i - EDITOR_MODE_NUM;
				if (color != editor.getLightColor()) {	// if hit button is not current light color, change color
					buttons[editor.getLightColor() + EDITOR_MODE_NUM].changeStatus();
					buttons[i].changeStatus();
					editor.setLightColor(color);
				}
			}
			else { // luminance button
				float luminance = editor.getLightLuminance();
				if (i == 11 && luminance > 0) luminance -= 0.1;		// minus
				else if (i == 12 && luminance < 1) luminance += 0.1;	// plus
				editor.setLightLuminance(luminance);
			}
			break;
		}
	}
}
