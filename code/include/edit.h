#pragma once
#include "map.h"
#include "belt.h"
#include <vector>

#define EDITOR_MODE_BELT 0
#define EDITOR_MODE_ARM 1
#define EDITOR_MODE_DELETE 2

class Editor {
private:
	int mode;
public:
	int getMode();
	void startDrawing();
	void endDrawing();

	void beltStartDrawing();
	void beltAddPoint(int z, int x);
	void beltEndDrawing();

};