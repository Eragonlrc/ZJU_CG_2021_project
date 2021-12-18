#include "map.h"

int Map::getType(int z, int x) {
	z += MAP_MAXZ, x += MAP_MAXX;
	if (z < 0 || z > MAP_MAXZ) return -1;
	if (x < 0 || x > MAP_MAXX) return -1;
	return map[z][x].type;
}

void* Map::getObj(int z, int x) {
	z += MAP_MAXZ, x += MAP_MAXX;
	if (z < 0 || z > MAP_MAXZ) return 0;
	if (x < 0 || x > MAP_MAXX) return 0;
	return map[z][x].obj;
}

bool Map::write(int z, int x, int type, const void* obj) {
	z += MAP_MAXZ, x += MAP_MAXX;
	if (z < 0 || z > MAP_MAXZ) return 0;
	if (x < 0 || x > MAP_MAXX) return 0;
	map[z][x].type = type, map[z][x].obj = (void*)obj;
	return 1;
}