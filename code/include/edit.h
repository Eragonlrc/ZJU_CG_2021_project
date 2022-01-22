#pragma once
#include "map.h"
#include "belt.h"
#include "skybox.h"
#include "arm.h"
#include "box.h"
#include "lighting.h"
#include <vector>

#define EDITOR_MODE_BELT 0
#define EDITOR_MODE_ARM 1
#define EDITOR_MODE_DELETE 2
#define EDITOR_MODE_LIGHTSOURCE 3

#define EDITOR_STATE_IDLE 0
#define EDITOR_STATE_DRAWING 1	// belt drawing, or arm waiting for from
#define EDITOR_STATE_ENDING 2	// arm waiting for to

class Editor {
private:
	int mode, state;
	void* current;
	std::vector<Point> mousePoints;
private: // belt
	Belt* prevBelt, * nextBelt;
	int firstIllegalBelt;
private:
	bool beltStartDrawing(int z, int x);
	void beltAddPoint(int z, int x);
	void beltUndoPoint();
	bool beltEndDrawing(bool cancel);
	void beltUpdateColor();

	bool armStartDrawing(int z, int x);
	bool armSetFrom(int z, int x);
	bool armSetTo(int z, int x);
	bool armEndDrawing(bool cancel);

	bool delPoint(int z, int x);
public:
	Editor();
	void draw();
	void drawMesh(int z, int x);

	int getMode();
	void changeMode(int _mode);
	int getState();
	
	bool startDrawing(int z, int x);
	void nextPoint(int z, int x);
	bool endDrawing(bool cancel = 0);
};