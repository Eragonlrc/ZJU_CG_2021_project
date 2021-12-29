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