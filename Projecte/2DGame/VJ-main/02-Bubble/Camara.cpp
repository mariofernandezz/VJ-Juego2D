#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Player.h"
#include "Game.h"
#include "Camara.h"

#define SCREEN_WIDTH 299
#define SCREEN_HEIGHT 224

#define MAX_X 100



Camara::Camara(glm::ivec2 playerPos, ShaderProgram &texProgram)
{
	this->texProgram = &texProgram; 
	texProgram.use();
	projection = glm::ortho(float(playerPos.x) - MAX_X, float(SCREEN_WIDTH - 1) + float(playerPos.x) - MAX_X, float(SCREEN_HEIGHT - 1), 0.f);
	texProgram.setUniformMatrix4f("projection", projection);
	posCamara = 0;//float(playerPos.x);
}

void Camara::update(glm::ivec2 playerPos, ShaderProgram &texProgram)
{
	texProgram.use();
	float novaPosCamara = float(playerPos.x) + MAX_X;
	if (novaPosCamara > posCamara) {
		projection = glm::ortho(float(playerPos.x) - MAX_X, float(SCREEN_WIDTH - 1) + float(playerPos.x) - MAX_X, float(SCREEN_HEIGHT - 1), 0.f);
		posCamara = novaPosCamara;
	}
	texProgram.setUniformMatrix4f("projection", projection);
}

float Camara::getPosCamara() {
	return posCamara - MAX_X;
}


