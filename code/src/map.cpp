#include "map.h"

Map::MapUnit::MapUnit() : type(MAP_BLANK), i(0), obj(0),
prev(POINTNULL), next(POINTNULL) {}

Map::Map() : firstObj(POINTNULL), lastObj(POINTNULL) {}

Map::MapUnit Map::getMap(int z, int x) {
	/*z += MAP_MAXZ, x += MAP_MAXX;*/
	if (z < 0 || z > MAP_MAXZ) return MapUnit();
	if (x < 0 || x > MAP_MAXX) return MapUnit();
	return map[z][x];
}

Map::MapUnit Map::getMap(Point p) {
	return getMap(p.first, p.second);
}

void Map::listIns(int z, int x) {
	Point p(z, x);
	if (firstObj == POINTNULL) {
		firstObj = lastObj = p;
		map[z][x].prev = map[z][x].next = POINTNULL;
	}
	else {
		map[z][x].prev = lastObj, map[z][x].next = POINTNULL;
		map[lastObj.first][lastObj.second].next = p;
		lastObj = p;
	}
}

void Map::listDel(int z, int x) {
	Point p(z, x);
	if (firstObj == p) firstObj = map[z][x].next;
	else map[map[z][x].prev.first][map[z][x].prev.second].next = map[z][x].next;
	if (lastObj == p) lastObj = map[z][x].prev;
	else map[map[z][x].next.first][map[z][x].next.second].prev = map[z][x].prev;
	map[z][x].prev = map[z][x].next = POINTNULL;
}

bool Map::write(int z, int x, int type, const void* obj, int index) {
	/*z += MAP_MAXZ, x += MAP_MAXX;*/
	if (z < 0 || z > MAP_MAXZ) return 0;
	if (x < 0 || x > MAP_MAXX) return 0;
	int prevType = map[z][x].type;
	map[z][x].type = type, map[z][x].obj = (void*)obj, map[z][x].i = index;
	if (index == 0) {
		if (prevType != MAP_BELT_DRAWING && type == MAP_BLANK) listDel(z, x);
		if(type != MAP_BELT_DRAWING && type != MAP_BLANK) listIns(z, x);
	}
	return 1;
}

Point Map::getFirst() { return firstObj; }
Point Map::getLast() { return lastObj; }

bool Map::checkEdge(float x, float y, float z) {
	int nextZ = (int)(z + 0.5);
	int nextX = (int)(x + 0.5);
	float dis;
	int nextType = getMap(nextZ, nextX).type;
	if (nextType == MAP_BLANK)	return (y >= 0.8);	// nothing, check floor
	switch (nextType) {
	case MAP_BELT_CORNER0:	// left top corner
		dis = sqrt(pow(z - (nextZ - 0.5), 2) + pow(x - (nextX - 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return (y >= 0.8);	// arc belt
		break;
	case MAP_BELT_CORNER1:	// right top corner
		dis = sqrt(pow(z - (nextZ - 0.5), 2) + pow(x - (nextX + 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return (y >= 0.8);	// arc belt
		break;
	case MAP_BELT_CORNER2:	// right bottom corner
		dis = sqrt(pow(z - (nextZ + 0.5), 2) + pow(x - (nextX + 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return (y >= 0.8);	// arc belt
		break;
	case MAP_BELT_CORNER3:	// left bottom corner
		dis = sqrt(pow(z - (nextZ + 0.5), 2) + pow(x - (nextX - 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return (y >= 0.8);	// arc belt
		break;
	case MAP_BELT_STRAIGHTX:
		if (abs(z - nextZ) > 0.4) return (y >= 0.8);
		break;
	case MAP_BELT_STRAIGHTZ:
		if (abs(x - nextX) > 0.4) return (y >= 0.8);
		break;
	case MAP_ARM:
		if (abs(z - nextZ) > 0.3 || abs(x - nextX) > 0.3)	return (y >= 0.8);
		return (y >= 2 + 0.8 - 0.001);
		break;
	case MAP_BOX:
		if (abs(z - nextZ) > 0.3 || abs(x - nextX) > 0.3)	return (y >= 0.8);
		return (y >= 0.5 + 0.8 - 0.001);
		break;
	default:	// other object, always fail
		return false;
		break;
	}
	return (y >= 0.5 + 0.8 - 0.001);	// check belt height
}

float Map::getFloor(float x, float z) {
	int nextZ = (int)(z + 0.5);
	int nextX = (int)(x + 0.5);
	float dis;
	int nextType = getMap(nextZ, nextX).type;
	if (nextType == MAP_BLANK)	return 0.8;	// nothing, check floor
	switch (nextType) {
	case MAP_BELT_CORNER0:	// left top corner
		dis = sqrt(pow(z - (nextZ - 0.5), 2) + pow(x - (nextX - 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return 0.8;	// arc belt
		break;
	case MAP_BELT_CORNER1:	// right top corner
		dis = sqrt(pow(z - (nextZ - 0.5), 2) + pow(x - (nextX + 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return 0.8;	// arc belt
		break;
	case MAP_BELT_CORNER2:	// right bottom corner
		dis = sqrt(pow(z - (nextZ + 0.5), 2) + pow(x - (nextX + 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return 0.8;	// arc belt
		break;
	case MAP_BELT_CORNER3:	// left bottom corner
		dis = sqrt(pow(z - (nextZ + 0.5), 2) + pow(x - (nextX - 0.5), 2));
		if (dis < 0.1 || dis > 0.9)	return 0.8;	// arc belt
		break;
	case MAP_BELT_STRAIGHTX:
		if (abs(z - nextZ) > 0.4) return 0.8;
		break;
	case MAP_BELT_STRAIGHTZ:
		if (abs(x - nextX) > 0.4) return 0.8;
		break;
	case MAP_ARM:
		if (abs(z - nextZ) > 0.3 || abs(x - nextX) > 0.3)	return 0.8;
		return 2 + 0.8;
		break;
	case MAP_BOX:
		if (abs(z - nextZ) > 0.3 || abs(x - nextX) > 0.3)	return 0.8;
		return 0.5 + 0.8;
		break;
	default:	// other object, always fail
		return 0.8;
		break;
	}
	return 0.5 + 0.8;	// check belt height
}