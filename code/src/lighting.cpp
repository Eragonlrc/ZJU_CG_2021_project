#include "lighting.h"

static const int lightHeight = 8;
static const float sphereR = 0.1;
static const float emission_default[4] = {0, 0, 0, 1};
static const float predefined_colors[][5][4] = {
	{ // white
		{0.9, 0.9, 0.9, 1},		// sphere color
		{1, 1, 1, 1},			// emission color
		{0, 0, 0, 1},			// ambient color
		{1, 1, 1, 1},			// diffuse color
		{1, 1, 1, 1}			// specular color
	},
	{ // red
		{0.9, 0, 0, 1},			// sphere color
		{1, 0, 0, 1},			// emission color
		{0, 0, 0, 1},			// ambient color
		{1, 0, 0, 1},			// diffuse color
		{1, 0, 0, 1}			// specular color
	},
	{ // green
		{0, 0.9, 0, 1},			// sphere color
		{0, 1, 0, 1},			// emission color
		{0, 0, 0, 1},			// ambient color
		{0, 1, 0, 1},			// diffuse color
		{0, 1, 0, 1}			// specular color
	},
	{ // blue
		{0, 0, 0.9, 1},			// sphere color
		{0, 0, 1, 1},			// emission color
		{0, 0, 0, 1},			// ambient color
		{0, 0, 1, 1},			// diffuse color
		{0, 0, 1, 1}			// specular color
	}
};

LightSource* LightSource::sources[MAX_LIGHTSOURCE] = { 0 };

int LightSource::nextId() {
	for (int i = 0; i < MAX_LIGHTSOURCE; i++) {
		if (!sources[i]) return i;
	}
	return -1;
}

void LightSource::drawAll() {
	for (int i = 0; i < MAX_LIGHTSOURCE; i++) {
		if (sources[i]) sources[i]->draw();
	}
}

void LightSource::enableAll() {
	for (int i = 0; i < MAX_LIGHTSOURCE; i++) {
		if (sources[i]) sources[i]->enable();
	}
}

void LightSource::delSource(int z, int x) {
	for (int i = 0; i < MAX_LIGHTSOURCE; i++) {
		if (sources[i] && sources[i]->pos[0] == x && sources[i]->pos[2] == z) delete sources[i];
	}
}

LightSource::LightSource(int z, int x,
	float sphereR, float sphereG, float sphereB, float sphereA,
	float emissionR, float emissionG, float emissionB, float emissionA,
	float ambientR, float ambientG, float ambientB, float ambientA,
	float diffuseR, float diffuseG, float diffuseB, float diffuseA,
	float specularR, float specularG, float specularB, float specularA):
	id(nextId()), pos{(float)x, lightHeight, (float)z, 1},
	sphere_color{ sphereR, sphereG, sphereB, sphereA },
	emission_color{ emissionR, emissionG, emissionB, emissionA },
	ambient_color{ ambientR, ambientG, ambientB, ambientA },
	diffuse_color{ diffuseR, diffuseG, diffuseB, diffuseA },
	specular_color{ specularR, specularG, specularB, specularA }{
	sources[id] = this;
}

LightSource::LightSource(int z, int x, int color, float luminance):
	LightSource(z, x,
				predefined_colors[color][0][0] * luminance, predefined_colors[color][0][1] * luminance, predefined_colors[color][0][2] * luminance, predefined_colors[color][0][3],
				predefined_colors[color][1][0] * luminance, predefined_colors[color][1][1] * luminance, predefined_colors[color][1][2] * luminance, predefined_colors[color][1][3],
				predefined_colors[color][2][0] * luminance, predefined_colors[color][2][1] * luminance, predefined_colors[color][2][2] * luminance, predefined_colors[color][2][3], 
				predefined_colors[color][3][0] * luminance, predefined_colors[color][3][1] * luminance, predefined_colors[color][3][2] * luminance, predefined_colors[color][3][3], 
				predefined_colors[color][4][0] * luminance, predefined_colors[color][4][1] * luminance, predefined_colors[color][4][2] * luminance, predefined_colors[color][4][3]){}

LightSource::~LightSource() {
	sources[id] = 0;
	glDisable(GL_LIGHT0 + id);
}

void LightSource::draw() {
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_color);
	glColor4fv(sphere_color);
	glPushMatrix();
	glTranslatef(pos[0], pos[1], pos[2]);
	glutSolidSphere(sphereR, 30, 30);
	glPopMatrix();
	glColor4f(1, 1, 1, 1);
	glMaterialfv(GL_FRONT, GL_EMISSION, emission_default);
	glEnable(GL_LIGHTING);
}

void LightSource::enable() {
	glLightfv(GL_LIGHT0 + id, GL_POSITION, pos);
	glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient_color);
	glLightfv(GL_LIGHT0 + id, GL_DIFFUSE, diffuse_color);
	glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular_color);
	glEnable(GL_LIGHT0 + id);
}