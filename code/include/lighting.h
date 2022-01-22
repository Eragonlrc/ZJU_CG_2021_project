#pragma once
#include "gl/glut.h"

#define MAX_LIGHTSOURCE 8

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
				float sphereR = 1, float sphereG = 1, float sphereB = 1, float sphereA = 1,
				float emissionR = 0, float emissionG = 0, float emissionB = 0, float emissionA = 1,
				float ambientR = 0, float ambientG = 0, float ambientB = 0, float ambientA = 1,
				float diffuseR = 1, float diffuseG = 1, float diffuseB = 1, float diffuseA = 1,
				float specularR = 1, float specularG = 1, float specularB = 1, float specularA = 1);
	LightSource(int z, int x, float* _sphere_color, float* _emission_color,
				float* _ambient_color, float* _diffuse_color, float* _specular_color);
	~LightSource();
	void draw();
	void enable();
};