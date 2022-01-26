#pragma once
#include <vector>
#include "texture.h"

#define MAP_MAXZ 1024
#define MAP_MAXX 1024

#define MAP_BLANK 0
#define MAP_BELT_CORNER0 1
#define MAP_BELT_CORNER1 2
#define MAP_BELT_CORNER2 3
#define MAP_BELT_CORNER3 4
#define MAP_BELT_STRAIGHTX 5
#define MAP_BELT_STRAIGHTZ 6
#define MAP_BELT_SINGLE 7
#define MAP_ARM 8
#define MAP_BOX 9
#define MAP_BELT_DRAWING 10
#define MAP_BORDER 11
#define MAP_ISBELT(type) ((type) >= MAP_BELT_CORNER0 && (type) <= MAP_BELT_SINGLE)

#define MAP_BORDER_ZMIN 462
#define MAP_BORDER_ZMAX 562
#define MAP_BORDER_XMIN 462
#define MAP_BORDER_XMAX 562

typedef std::pair<int, int> Point;
const Point POINTNULL(MAP_MAXZ, MAP_MAXX);

class Map {
public:
	struct MapUnit {
		int type;
		void* obj;
		int i;						// the index of point in belt object
		Point prev, next;			// object list
		MapUnit();
	};
private:
	MapUnit map[MAP_MAXZ][MAP_MAXX];
	Point firstObj, lastObj;
	void listIns(int z, int x);		// insert to object list tail
	void listDel(int z, int x);		// delete from object list
private: // ground
	TexLoader groundTex;
	GLuint groundListId;
	void drawGroundList();
public:
	Map();
	void init();

	MapUnit getMap(Point p);
	MapUnit getMap(int z, int x);
	bool write(int z, int x, int type, const void* obj, int index = 0);	// write map[z][x], delete if type == MAP_BLANK, otherwise insert
	Point getFirst();
	Point getLast();
	bool checkEdge(float x, float y, float z);	// collision detection; true -> pass; false -> fail
	float getFloor(float x, float z);
	void drawGround();
};
