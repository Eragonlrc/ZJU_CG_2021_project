#include "edit.h"

extern Map map;

Editor::Editor() : mode(EDITOR_MODE_BELT), current(NULL),
				   prevBelt(NULL), nextBelt(NULL), firstIllegalBelt(-1) {}

int Editor::getMode() {
	return mode;
}

void Editor::changeMode(int _mode) {
	mode = _mode;
}

void Editor::drawMesh(int z, int x) {
	float i;
	glBegin(GL_LINES);
	for (i = -0.5; i < BOX_SIZE; i += 1) {	// 平行于x轴的网格
		glVertex3f(0, 0, i);
		glVertex3f(BOX_SIZE, 0, i);
	}
	for (i = -0.5; i < BOX_SIZE; i++) {	// 平行于z轴的网格
		glVertex3f(i, 0, 0);
		glVertex3f(i, 0, BOX_SIZE);
	}
	glColor3f(1.0, 0, 0);	// 当前鼠标所在网格用红色突出
	glVertex3f(x - 0.5, 0, z - 0.5); glVertex3f(x + 0.5, 0, z - 0.5);	// 上
	glVertex3f(x - 0.5, 0, z + 0.5); glVertex3f(x + 0.5, 0, z + 0.5);	// 下
	glVertex3f(x - 0.5, 0, z + 0.5); glVertex3f(x - 0.5, 0, z - 0.5);	// 左
	glVertex3f(x + 0.5, 0, z + 0.5); glVertex3f(x + 0.5, 0, z - 0.5);	// 右
	glEnd();
}

bool Editor::startDrawing(int z, int x) {
	switch (mode) {
		case EDITOR_MODE_BELT: {
			return beltStartDrawing(z, x);
			break;
		}
	}
}

bool Editor::endDrawing(bool cancel) {
	switch (mode) {
		case EDITOR_MODE_BELT: {
			return beltEndDrawing(cancel);
			break;
		}
	}
}

bool Editor::beltStartDrawing(int z, int x) {
	current = new Belt();
	Belt* belt = (Belt*)current;
	Map::MapUnit mu = map.getMap(z, x);
	belt->setColor(BELT_COLOR_DRAWING);
	if (MAP_ISBELT(mu.type) && mu.i == ((Belt*)(mu.obj))->getLength() - 1) { // merge after previous belt, do not push first point
		prevBelt = ((Belt*)(mu.obj));
		prevBelt->setColor(BELT_COLOR_DRAWING);
	}
	else if (mu.type != MAP_BLANK) return 0;
	else {
		belt->pushPoint(z, x);
		map.write(z, x, MAP_BELT_DRAWING, belt, 0);
	}
	return 1;
}

void Editor::beltAddPoint(int z, int x) {
	Belt* belt = (Belt*)current;
	Map::MapUnit mu = map.getMap(z, x);
	belt->pushPoint(z, x);
	if (nextBelt) { // previous point intersects with the start of another belt
		nextBelt->setColor(BELT_COLOR_DEFAULT);
		nextBelt = NULL;
		firstIllegalBelt = belt->getLength() - 2;
		belt->setColor(BELT_COLOR_WARNING);
	}
	if (firstIllegalBelt == -1 && mu.type != MAP_BLANK) { // no previous illegal, and current one intersects
		if (MAP_ISBELT(mu.type) && mu.i == 0) { // intersect with the start of another belt
			nextBelt = (Belt*)(mu.obj);
			nextBelt->setColor(BELT_COLOR_DRAWING);
		}
		else { // intersect with other objects
			firstIllegalBelt = belt->getLength() - 1;
			belt->setColor(BELT_COLOR_WARNING);
		}
	}
	if (mu.type == MAP_BLANK) map.write(z, x, MAP_BELT_DRAWING, belt, belt->getLength() - 1);
}

void Editor::beltUndoPoint() {
	Belt* belt = (Belt*)current;
	Map::MapUnit mu = map.getMap(belt->getPoint(belt->getLength() - 1));
	if (firstIllegalBelt == belt->getLength() - 1) { // last belt is the first illegal one
		firstIllegalBelt = -1;
		belt->setColor(BELT_COLOR_DRAWING);
		if (MAP_ISBELT(mu.type) && mu.i == 0) { // previous point intersects with the start of another belt
			nextBelt = (Belt*)(mu.obj);
			nextBelt->setColor(BELT_COLOR_DRAWING);
		}
	}
	else if (mu.type == MAP_BELT_DRAWING)
		map.write(belt->getPoint(belt->getLength() - 1).first, belt->getPoint(belt->getLength() - 1).second,
								 MAP_BLANK, NULL);
	belt->undoPoint();
}

bool Editor::beltEndDrawing(bool cancel) {
	Belt* belt = (Belt*)current;
	if (cancel) { // cancel drawing
		if (nextBelt) {
			nextBelt->setColor(BELT_COLOR_DEFAULT);
			nextBelt = NULL;
		}
		delete belt;
		current = NULL;
		if (prevBelt) {
			prevBelt->setColor(BELT_COLOR_DEFAULT);
			prevBelt = NULL;
		}
		if (nextBelt) {
			nextBelt->setColor(BELT_COLOR_DEFAULT);
			nextBelt = NULL;
		}
		firstIllegalBelt = -1;
		return 1;
	}
	if (firstIllegalBelt != -1) return 0; // illegal points exist
	else { // normally finish drawing
		belt->updateMap(); // update map to actual belt
		belt->setColor(BELT_COLOR_DEFAULT);
		if (nextBelt) { // merge with next belt
			nextBelt->setColor(BELT_COLOR_DEFAULT);
			belt->merge(nextBelt);
			nextBelt = NULL;
		}
		if (prevBelt) { // merge with previous belt
			prevBelt->setColor(BELT_COLOR_DEFAULT);
			prevBelt->merge(nextBelt);
			prevBelt = NULL;
		}
		current = NULL;
		return 1;
	}
}