#include "lighting.h"

static const int lightHeight = 8;
static const float sphereR = 0.1;
static const float emission_default[4] = {0, 0, 0, 1};

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

LightSource::LightSource(int z, int x, float* _sphere_color, float* _emission_color,
						 float* _ambient_color, float* _diffuse_color, float* _specular_color):
	LightSource(z, x,
				_sphere_color[0], _sphere_color[1], _sphere_color[2], _sphere_color[3],
				_emission_color[0], _emission_color[1], _emission_color[2], _emission_color[3],
				_ambient_color[0], _ambient_color[1], _ambient_color[2], _ambient_color[3],
				_diffuse_color[0], _diffuse_color[1], _diffuse_color[2], _diffuse_color[3],
				_specular_color[0], _specular_color[1], _specular_color[2], _specular_color[3]){}

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