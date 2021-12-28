#pragma once
#include "map.h"
#include "belt.h"
#include "skybox.h"
#include "arm.h"
#include "box.h"

#define EDITOR_MODE_BELT 0
#define EDITOR_MODE_ARM 1
#define EDITOR_MODE_DELETE 2

class Editor {
private:
	int mode;
	void* current;
private: // belt
	Belt* prevBelt, * nextBelt;
	int firstIllegalBelt;
public:
	Editor();
	void draw();

	int getMode();
	void changeMode(int _mode);
	void drawMesh(int z, int x);

	bool startDrawing(int z, int x);
	bool endDrawing(bool cancel);

	bool beltStartDrawing(int z, int x);
	void beltAddPoint(int z, int x);
	void beltUndoPoint();
	bool beltEndDrawing(bool cancel);
	bool beltDelete(int z, int x);

	bool armStartDrawing(int z, int x);
	void armSetFrom(int z, int x);
	void armSetTo(int z, int x);
	bool armEndDrawing(bool cancel);
	bool armDelete(int z, int x);
};