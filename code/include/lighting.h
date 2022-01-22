#pragma once
#include "gl/glut.h"

#define MAX_LIGHTSOURCE 8

#define LIGHT_COLOR_NUM 7
#define LIGHT_COLOR_WHITE 0
#define LIGHT_COLOR_RED 1
#define LIGHT_COLOR_GREEN 2
#define LIGHT_COLOR_BLUE 3
#define LIGHT_COLOR_YELLOW 4
#define LIGHT_COLOR_MAGENTA 5
#define LIGHT_COLOR_CYAN 6

class LightSource {
private:
	static LightSource* sources[MAX_LIGHTSOURCE];
public:
	static int nextId();
	static void drawAll();
	static void enableAll();
	static void delSource(int z, int x);
private:
	int id;
	float pos[4];
	float sphere_color[4];
	float emission_color[4];
	float ambient_color[4];
	float diffuse_color[4];
	float specular_color[4];
public:
	LightSource(int z, int x,
				float sphereR, float sphereG, float sphereB, float sphereA,
				float emissionR, float emissionG, float emissionB, float emissionA,
				float ambientR, float ambientG, float ambientB, float ambientA,
				float diffuseR, float diffuseG, float diffuseB, float diffuseA,
				float specularR, float specularG, float specularB, float specularA);
	LightSource(int z, int x, int color = LIGHT_COLOR_WHITE, float luminance = 1);
	~LightSource();
	void draw();
	void enable();
};