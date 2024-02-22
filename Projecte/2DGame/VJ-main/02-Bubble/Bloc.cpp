#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Bloc.h"




enum BlocAnims
{
	TERRA, MAO, BLOC_SORPRESA, BLOC_OBERT, BLOC_DUR, TUBITO_1, TUBITO_2, TUBITO_3, TUBITO_4, CASTELL_A, CASTELL_B, CASTELL_C, CASTELL_D, 
	CASTELL_E, CASTELL_F, CASTELL_G, CASTELL_H, CASTELL_I, CASTELL_J
};


void Bloc::init(int blocType, ShaderProgram& shaderProgram, string tileSheetFile)
{
	spritesheet.loadFromFile("images/tilebueno.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f/18.f, 1.f/4.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(19);

	sprite->setAnimationSpeed(TERRA, 1);
	sprite->addKeyframe(TERRA, glm::vec2(7.f / 18.f, 1.f / 4.f));

	sprite->setAnimationSpeed(MAO, 1);
	sprite->addKeyframe(MAO, glm::vec2(5.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(BLOC_SORPRESA, 2);
	sprite->addKeyframe(BLOC_SORPRESA, glm::vec2(7.f / 18.f, 0.f / 4.f));
	sprite->addKeyframe(BLOC_SORPRESA, glm::vec2(8.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(BLOC_OBERT, 1);
	sprite->addKeyframe(BLOC_OBERT, glm::vec2(8.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(BLOC_DUR, 1);
	sprite->addKeyframe(BLOC_DUR, glm::vec2(15.f / 18.f, 1.f / 4.f));

	sprite->setAnimationSpeed(TUBITO_1, 1);
	sprite->addKeyframe(TUBITO_1, glm::vec2(16.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(TUBITO_2, 1);
	sprite->addKeyframe(TUBITO_2, glm::vec2(17.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(TUBITO_3, 1);
	sprite->addKeyframe(TUBITO_3, glm::vec2(16.f / 18.f, 1.f / 4.f));

	sprite->setAnimationSpeed(TUBITO_4, 1);
	sprite->addKeyframe(TUBITO_4, glm::vec2(17.f / 18.f, 1.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_A, 1);
	sprite->addKeyframe(CASTELL_A, glm::vec2(6.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_B, 1);
	sprite->addKeyframe(CASTELL_B, glm::vec2(5.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_C, 1);
	sprite->addKeyframe(CASTELL_C, glm::vec2(1.f / 18.f, 3.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_D, 1);
	sprite->addKeyframe(CASTELL_D, glm::vec2(11.f / 18.f, 2.f / 4.f));
	
	sprite->setAnimationSpeed(CASTELL_E, 1);
	sprite->addKeyframe(CASTELL_E, glm::vec2(0.f / 18.f, 3.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_F, 1);
	sprite->addKeyframe(CASTELL_F, glm::vec2(2.f / 18.f, 3.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_G, 1);
	sprite->addKeyframe(CASTELL_G, glm::vec2(15.f / 18.f, 0.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_H, 1);
	sprite->addKeyframe(CASTELL_H, glm::vec2(5.f / 18.f, 1.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_I, 1);
	sprite->addKeyframe(CASTELL_I, glm::vec2(8.f / 18.f, 3.f / 4.f));

	sprite->setAnimationSpeed(CASTELL_J, 1);
	sprite->addKeyframe(CASTELL_J, glm::vec2(7.f / 18.f, 3.f / 4.f));


	sprite->changeAnimation(0);
	
	Type = blocType;
	creaCastell();
	assignaAnimació();
	
}

void Bloc::render(float x, float y)
{
	sprite->update(50);
	if (Type != 0) {
		sprite->setPosition(glm::vec2(x, y));
		sprite->render();
	}
	
}

int Bloc::collision(bool isSupermario){
	if (Type == 3) {
		Type = 11;
		assignaAnimació();
		return 1;
	}
	else if(Type == 4){
		Type = 11;
		assignaAnimació();
		return 2;
	}
	else if (Type == 5) {
		Type = 11;
		assignaAnimació();
		return 3;
	}
	else if (Type == 2 && isSupermario) {
		Type = 0;
		assignaAnimació();
		return 0;
	}
}

int Bloc::getType() {
	if(Type <= 11) return Type;
	return 0;
}

void Bloc::assignaAnimació() {
	switch (Type) {
	case 1: sprite->changeAnimation(TERRA);
		break;
	case 2: sprite->changeAnimation(MAO);
		break;
	case 3: sprite->changeAnimation(BLOC_SORPRESA);
		break;
	case 4: sprite->changeAnimation(BLOC_SORPRESA);
		break;
	case 5: sprite->changeAnimation(BLOC_SORPRESA);
		break;
	case 6: sprite->changeAnimation(BLOC_DUR);
		break;
	case 7: sprite->changeAnimation(TUBITO_1);
		break;
	case 8: sprite->changeAnimation(TUBITO_2);
		break;
	case 9: sprite->changeAnimation(TUBITO_3);
		break;
	case 10: sprite->changeAnimation(TUBITO_4);
		break;
	case 11: sprite->changeAnimation(BLOC_OBERT);
		break;
	}
}

void Bloc::creaCastell() {
	switch (Type - int('A') + int('0')) {
	case 0: sprite->changeAnimation(CASTELL_A);
		break;
	case 1: sprite->changeAnimation(CASTELL_B);
		break;
	case 2: sprite->changeAnimation(CASTELL_C);
		break;
	case 3: sprite->changeAnimation(CASTELL_D);
		break;
	case 4: sprite->changeAnimation(CASTELL_E);
		break;
	case 5: sprite->changeAnimation(CASTELL_F);
		break;
	case 6: sprite->changeAnimation(CASTELL_G);
		break;
	case 7: sprite->changeAnimation(CASTELL_H);
		break;
	case 8: sprite->changeAnimation(CASTELL_I);
		break;
	case 9: sprite->changeAnimation(CASTELL_J);
		break;
	}
}