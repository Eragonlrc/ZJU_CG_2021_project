#include "edit.h"

extern Map map;

void Editor::draw() {
	if (!current) return;
	switch (mode) {
	case EDITOR_MODE_BELT:
		((Belt*)current)->draw();
		break;
	case EDITOR_MODE_ARM:
		((Arm*)current)->draw();
		break;
	}
}

Editor::Editor() : mode(EDITOR_MODE_BELT), state(EDITOR_STATE_IDLE), current(NULL),
				   prevBelt(NULL), nextBelt(NULL), firstIllegalBelt(-1) {}

int Editor::getMode() {
	return mode;
}

void Editor::changeMode(int _mode) {
	mode = _mode;
}

int Editor::getState() {
	return state;
}

void Editor::drawMesh(int z, int x) {
	float i;
	glDisable(GL_LIGHTING);
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
	glColor3f(1.0, 1.0, 1.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

bool Editor::startDrawing(int z, int x) {
	bool ret = 0;
	switch (mode) {
		case EDITOR_MODE_BELT: {
			ret = beltStartDrawing(z, x);
			if(ret) beltUpdateColor();
			break;
		}
		case EDITOR_MODE_ARM: {
			ret = armStartDrawing(z, x);
			break;
		}
		case EDITOR_MODE_DELETE: {
			return delPoint(z, x);
		}
	}
	if (ret) state = EDITOR_STATE_DRAWING;
	return ret;
}

void Editor::nextPoint(int z, int x) {
	switch (mode) {
		case EDITOR_MODE_BELT: {
			Belt* belt = (Belt*)current;
			if (belt->getLength() == 1 && prevBelt && prevBelt->getPoint(prevBelt->getLength() - 1) == Point(z, x) || // merge with previous belt, and undo the first new point
				belt->getLength() > 1 && belt->getPoint(belt->getLength() - 2) == Point(z, x)) { // normal case, length >= 2 and undo the last point
				beltUndoPoint();
			}
			else beltAddPoint(z, x); // add point
			beltUpdateColor();
			break;
		}
		case EDITOR_MODE_ARM: {
			if (state == EDITOR_STATE_DRAWING) {
				if(armSetFrom(z, x)) state = EDITOR_STATE_ENDING;
			}
			else {
				if(armSetTo(z, x)) endDrawing();
			}
			break;
		}
	}
}

bool Editor::endDrawing(bool cancel) {
	bool ret = 0;
	switch (mode) {
		case EDITOR_MODE_BELT: {
			ret = beltEndDrawing(cancel);
			break;
		}
		case EDITOR_MODE_ARM: {
			ret = armEndDrawing(cancel);
			break;
		}
	}
	if(ret) state = EDITOR_STATE_IDLE;
	return ret;
}

bool Editor::beltStartDrawing(int z, int x) {
	Belt* belt;
	Map::MapUnit mu = map.getMap(z, x);
	if (MAP_ISBELT(mu.type) && mu.i == ((Belt*)(mu.obj))->getLength() - 1 || mu.type == MAP_BLANK) {
		current = new Belt();
		belt = (Belt*)current;
		belt->pushPoint(z, x);
		if(mu.type == MAP_BLANK) map.write(z, x, MAP_BELT_DRAWING, belt, 0); // normal case, push first point
		else prevBelt = ((Belt*)(mu.obj)); // merge after previous belt
	}
	else return 0; // intersects, do not start drawing
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
	}
	if (firstIllegalBelt == -1 && mu.type != MAP_BLANK) { // no previous illegal, and current one intersects
		if (MAP_ISBELT(mu.type) && mu.i == 0 &&													// intersect with the start of another belt
			((Belt*)(mu.obj))->getPoint(((Belt*)(mu.obj))->getLength() - 1) != Point(z, x)) {	// and the other belt is not a loop
			nextBelt = (Belt*)(mu.obj);
		}
		else if (mu.type == MAP_BELT_DRAWING && mu.i == 0) {
			nextBelt = belt;
		}
		else { // intersect with other objects
			firstIllegalBelt = belt->getLength() - 1;
		}
	}
	if (mu.type == MAP_BLANK) map.write(z, x, MAP_BELT_DRAWING, belt, belt->getLength() - 1);
}

void Editor::beltUndoPoint() {
	Belt* belt = (Belt*)current;
	Map::MapUnit mu = map.getMap(belt->getPoint(belt->getLength() - 1));
	if (firstIllegalBelt == belt->getLength() - 1) { // last belt is the first illegal one
		firstIllegalBelt = -1;
	}
	else if (firstIllegalBelt == belt->getLength() - 2) {
		Map::MapUnit prevmu = map.getMap(belt->getPoint(belt->getLength() - 2));
		if ((MAP_ISBELT(prevmu.type) || prevmu.type == MAP_BELT_DRAWING) && prevmu.i == 0 &&									// previous point intersects with the start of another belt
			((Belt*)(prevmu.obj))->getPoint(((Belt*)(prevmu.obj))->getLength() - 1) != belt->getPoint(belt->getLength() - 2)) {	// and the other belt is not a loop
			nextBelt = (Belt*)(prevmu.obj);
			firstIllegalBelt = -1;
		}
	}
	else if (mu.obj == nextBelt) nextBelt = NULL;
	if (mu.type == MAP_BELT_DRAWING && mu.i == belt->getLength() - 1)
		map.write(belt->getPoint(belt->getLength() - 1).first, belt->getPoint(belt->getLength() - 1).second, MAP_BLANK, NULL);
	belt->undoPoint();
}

bool Editor::beltEndDrawing(bool cancel) {
	Belt* belt = (Belt*)current;
	if (cancel) { // cancel drawing
		if (prevBelt) {
			prevBelt->setColor(BELT_COLOR_DEFAULT);
			prevBelt = NULL;
		}
		if (nextBelt) {
			nextBelt->setColor(BELT_COLOR_DEFAULT);
			nextBelt = NULL;
		}
		belt->delMap(1);
		delete belt;
		current = NULL;
		firstIllegalBelt = -1;
		return 1;
	}
	if (firstIllegalBelt != -1) return 0; // illegal points exist
	else { // normally finish drawing
		belt->setColor(BELT_COLOR_DEFAULT);
		belt->delMap(1);
		if (nextBelt) { // merge with next belt
			if (nextBelt != belt) {
				nextBelt->setColor(BELT_COLOR_DEFAULT);
				nextBelt->delMap();
				belt->merge(nextBelt);
			}
			if (prevBelt == nextBelt) prevBelt = NULL;
			nextBelt = NULL;
		}
		if (prevBelt) { // merge with previous belt
			prevBelt->setColor(BELT_COLOR_DEFAULT);
			prevBelt->delMap();
			prevBelt->merge(belt);
			prevBelt->updateMap();
			prevBelt = NULL;
		}
		else belt->updateMap(); // update map to actual belt
		current = NULL;
		return 1;
	}
}

void Editor::beltUpdateColor() {
	((Belt*)current)->setColor(firstIllegalBelt == -1 ? BELT_COLOR_DRAWING: BELT_COLOR_WARNING);
	if (prevBelt) prevBelt->setColor(BELT_COLOR_DRAWING);
	if (nextBelt) nextBelt->setColor(BELT_COLOR_DRAWING);
}

bool Editor::armStartDrawing(int z, int x) {
	Map::MapUnit mu = map.getMap(z, x);
	if (mu.type != MAP_BLANK) return 0;		// 如果当前网格非空，绘制失败
	current = new Arm(z, x, 0, 0);
	Arm* arm = (Arm *)current;
	arm->setColor(ARM_COLOR_DRAWING);
	return 1;
}

bool Editor::armSetFrom(int z, int x) {
	Arm* arm = (Arm*)current;
	Point pos = arm->getPosition();
	if (z == pos.first - 1 && x == pos.second)	arm->setFrom(2);	// 上
	else if (z == pos.first + 1 && x == pos.second)	arm->setFrom(0);	// 下
	else if (z == pos.first && x == pos.second - 1)	arm->setFrom(3);	// 左
	else if (z == pos.first && x == pos.second + 1)	arm->setFrom(1);	// 右
	else return 0;
	return 1;
}

bool Editor::armSetTo(int z, int x) {
	Arm* arm = (Arm*)current;
	Point pos = arm->getPosition();
	if (z == pos.first - 1 && x == pos.second)	arm->setTo(2);	// 上
	else if (z == pos.first + 1 && x == pos.second)	arm->setTo(0);	// 下
	else if (z == pos.first && x == pos.second - 1)	arm->setTo(3);	// 左
	else if (z == pos.first && x == pos.second + 1)	arm->setTo(1);	// 右
	else return 0;
	return 1;
}

bool Editor::armEndDrawing(bool cancel) {
	Arm* arm = (Arm*)current;
	Point pos = arm->getPosition();
	if (cancel) {
		delete arm;
		current = NULL;
		return 1;
	}
	else {
		arm->setColor(ARM_COLOR_DEFAULT);
		map.write(pos.first, pos.second, MAP_ARM, current);
		current = NULL;
	}
	return 1;
}

bool Editor::delPoint(int z, int x) {
	Map::MapUnit mu = map.getMap(z, x);
	if (MAP_ISBELT(mu.type)) {
		Belt* belt = (Belt*)(mu.obj);
		belt->delPoint(mu.i);
		if (belt->getLength() == 0) delete belt;
		return 1;
	}
	else if (mu.type == MAP_ARM) {
		Arm* arm = (Arm*)(mu.obj);
		map.write(z, x, MAP_BLANK, NULL);
		delete arm;
		return 1;
	}
	return 0;
}