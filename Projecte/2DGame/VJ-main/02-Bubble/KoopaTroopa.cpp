
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "KoopaTroopa.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define VELOCITY_MAX 200
#define ACCELERATION 50
#define VELOCITY_MIN 50
#define GRAVITY 350
#define TERMINAL_VELOCITY 300


#define FACTOR_IGUALADOR 0.1



enum KoopaTroopaAnims
{
	MOVE_LEFT, MOVE_RIGHT
};


void KoopaTroopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/KoopaTroopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 32), glm::vec2(1.f/5.f, 1.f/3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4, 1.f / 3.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2, 1.f / 3.f));



	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.f));


	closca = new CloscaKoopaTroopa;
	closca->init(tileMapPos, shaderProgram);

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
	sprite->changeAnimation(MOVE_RIGHT);
	speed = 100;
	speed_y = 0;
	alive = true;
	despert = false;
}

void KoopaTroopa::update(int deltaTime, glm::ivec2 posPlayer)
{
	if (abs(posPlayer.x - posGoomba.x) < 200) despert = true;
	if (despert) {
		if (alive) {
			sprite->update(deltaTime);
			if (speed > 0 && mapInteractive->collisionMoveRight(posGoomba, glm::ivec2(24, 32))) {
				sprite->changeAnimation(MOVE_LEFT);
				speed = -100 * FACTOR_IGUALADOR;
			}
			else if (speed < 0 && mapInteractive->collisionMoveLeft(posGoomba, glm::ivec2(24, 32))) {
				sprite->changeAnimation(MOVE_RIGHT);
				speed = 100;
			}

			posGoomba.x += speed * float(deltaTime) / 1000.f;


			if (mapInteractive->collisionMoveDown(posGoomba, glm::ivec2(24, 32), &posGoomba.y)) {
				speed_y = 0;
			}
			else speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;

			if (speed_y > TERMINAL_VELOCITY) speed_y = TERMINAL_VELOCITY;
			posGoomba.y += speed_y * float(deltaTime) / 1000.f;


			if (killed(posPlayer)) {
				closca->setPosition(glm::ivec2(glm::vec2(float(posGoomba.x), float(posGoomba.y + 8.f))));
				closca->setTileMapInteractive(mapInteractive);
				closca->setPlayer(player);
				player->rebota();
				player->enemyKilled();
				alive = false;
			}
			else if (collidesWithPlayer(posPlayer)) {
				player->hurt();
			}
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));

			if (posGoomba.y > 190) {
				closca->setPosition(glm::ivec2(glm::vec2(float(posGoomba.x), float(posGoomba.y + 8.f))));
				closca->setTileMapInteractive(mapInteractive);
				closca->setPlayer(player);
				alive = false;
			}
		}
		else closca->update(deltaTime, posPlayer);
	}
}

void KoopaTroopa::render()
{
	if (alive) sprite->render();
	else closca->render();
}

void KoopaTroopa::setTileMapInteractive(TileMapInteractive* tileMapInteractive)
{
	mapInteractive = tileMapInteractive;
}

void KoopaTroopa::setPosition(const glm::vec2& pos)
{
	posGoomba = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

//
bool KoopaTroopa::collidesWithPlayer(glm::ivec2 posPlayer) {
	if (abs(posPlayer.y - posGoomba.y) > 32) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 16) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 16) <= posGoomba.x)) return true;
	else return false;
}


bool KoopaTroopa::killed(glm::ivec2 posPlayer) {
	if (posPlayer.y > (posGoomba.y - 25) || posPlayer.y < (posGoomba.y - 32)) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 32) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 32) <= posGoomba.x)) return true;
	else return false;
}

void KoopaTroopa::setPlayer(Player* player) {
	if(player != NULL) this->player = player;
}
bool KoopaTroopa::isCloscaMooving() {
	if (alive) return false;
	return closca->moving();
}

glm::ivec2 KoopaTroopa::getPos() {
	if (alive) return posGoomba;
	else return closca->getPos();
}