#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "PowerUp.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define VELOCITY_MAX 200
#define ACCELERATION 50
#define VELOCITY_MIN 50
#define GRAVITY 350

#define FACTOR_IGUALADOR 0.1



enum PowerUpAnims
{
	STAR, BOLET, COIN
};


void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type)
{

	spritesheet.loadFromFile("images/up.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 1), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(STAR, 1);
	sprite->addKeyframe(STAR, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(BOLET, 1);
	sprite->addKeyframe(BOLET, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(COIN, 1);
	sprite->addKeyframe(COIN, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	Type = type;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));

	switch (Type) {
	case 1:
		sprite->changeAnimation(COIN);
		break;
	case 2:
		sprite->changeAnimation(BOLET);
		break;
	case 3:
		sprite->changeAnimation(STAR);
		break;
	}
	speed = 100;
	speed_y = 0;
	alive = true;
}

void PowerUp::update(int deltaTime, glm::ivec2 posPlayer)
{
	if (alive && Type!= 1) {

		sprite->update(deltaTime);
		if (speed > 0 && mapInteractive->collisionMoveRight(posGoomba, glm::ivec2(24, 24))) {
			speed = -100 * FACTOR_IGUALADOR;
		}
		else if (speed < 0 && mapInteractive->collisionMoveLeft(posGoomba, glm::ivec2(24, 24))) {
			speed = 100;
		}
		posGoomba.x += speed * deltaTime / 1000.f;

		if (killed(posPlayer) || collidesWithPlayer(posPlayer)) {
			alive = false;
			player->powerUp(Type);
		}

		if (mapInteractive->collisionMoveDown(posGoomba, glm::ivec2(24, 24), &posGoomba.y)) {
			speed_y = 0;
		}
		else speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;


		posGoomba.y += speed_y * float(deltaTime) / 1000.f;

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));

	}
	else if (alive) {
		sprite->update(deltaTime);
		speed_y = -200;
		posGoomba.y += speed_y * float(deltaTime) / 1000.f;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
		if (posGoomba.y <= posMaxMoneda) {
			alive = false;
		}
	}
	//if(Type == 1) player->getCoin();
}

void PowerUp::render()
{
	if(alive) sprite->render();
}

void PowerUp::setTileMapInteractive(TileMapInteractive* tileMapInteractive)
{
	mapInteractive = tileMapInteractive;
}

void PowerUp::setPosition(const glm::vec2& pos)
{
	posGoomba = pos;
	posMaxMoneda = float(posGoomba.y - 20);
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

bool PowerUp::collidesWithPlayer(glm::ivec2 posPlayer) {
	if (abs(posPlayer.y - posGoomba.y) > 32) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 24) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 24) <= posGoomba.x)) return true;
	else return false;
}


bool PowerUp::killed(glm::ivec2 posPlayer) {
	if (posPlayer.y > (posGoomba.y - 17) || posPlayer.y < (posGoomba.y - 24)) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 24) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 24) <= posGoomba.x)) return true;
	else return false;
}

void PowerUp::setPlayer(Player* player) {
	this->player = player;
}
