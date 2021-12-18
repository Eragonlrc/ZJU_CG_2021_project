#pragma once

#define MAP_MAXZ 128
#define MAP_MAXX 128

#define MAP_BLANK 0
#define MAP_BELT_CORNER0 1
#define MAP_BELT_CORNER1 2
#define MAP_BELT_CORNER2 3
#define MAP_BELT_CORNER3 4
#define MAP_BELT_STRAIGHTX 5
#define MAP_BELT_STRAIGHTZ 6
#define MAP_ARM 7
#define MAP_MACHINE 8

class Map {
private:
	struct MapUnit {
		int type;
		void* obj;
		MapUnit() : type(MAP_BLANK), obj(0) {}
	} map[MAP_MAXZ >> 1][MAP_MAXX >> 1];
public:
	int getType(int z, int x);
	void* getObj(int z, int x);
	bool write(int z, int x, int type, const void* obj);
};

