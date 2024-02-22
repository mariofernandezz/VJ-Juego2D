#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "CloscaKoopaTroopa.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define VELOCITY_MAX 200
#define ACCELERATION 50
#define VELOCITY_MIN 50
#define GRAVITY 350
#define TERMINAL_VELOCITY 300





enum CloscaKoopaTroopaAnims
{
	CLOSCA_ESQUERRA, CLOSCA_DRETA, CLOSCA_QUIETA
};


void CloscaKoopaTroopa::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/KoopaTroopa.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(1.f / 5.f, 1.f / 3.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);


	sprite->setAnimationSpeed(CLOSCA_ESQUERRA, 8);
	sprite->addKeyframe(CLOSCA_ESQUERRA, glm::vec2(0.f, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_ESQUERRA, glm::vec2(0.2, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_ESQUERRA, glm::vec2(0.4, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_ESQUERRA, glm::vec2(0.6, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_ESQUERRA, glm::vec2(0.8, 2.f / 3.f));

	sprite->setAnimationSpeed(CLOSCA_DRETA, 8);
	sprite->addKeyframe(CLOSCA_DRETA, glm::vec2(0.f, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_DRETA, glm::vec2(0.2, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_DRETA, glm::vec2(0.4, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_DRETA, glm::vec2(0.6, 2.f / 3.f));
	sprite->addKeyframe(CLOSCA_DRETA, glm::vec2(0.8, 2.f / 3.f));

	sprite->setAnimationSpeed(CLOSCA_QUIETA, 8);
	sprite->addKeyframe(CLOSCA_QUIETA, glm::vec2(0.f, 2.f / 3.f));


	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
	sprite->changeAnimation(CLOSCA_QUIETA);
	speed = 0;
	speed_y = 0;
	quieta = true;
	alive = true;
}

void CloscaKoopaTroopa::update(int deltaTime, glm::ivec2 posPlayer)
{
	if (alive) {
		sprite->update(deltaTime);
		if (quieta) {
			if (killed(posPlayer)) {
				if (posPlayer.x + 16 < posGoomba.x + 12) {
					speed = 150;
					sprite->changeAnimation(CLOSCA_DRETA);

				}
				else {
					speed = -150;
					sprite->changeAnimation(CLOSCA_ESQUERRA);
				}
				player->rebota();
				quieta = false;
			}
			else if (collidesWithPlayer(posPlayer)) {
				if (posPlayer.x + 16 < posGoomba.x + 12) {
					speed = 150;
					sprite->changeAnimation(CLOSCA_DRETA);

				}
				else {
					speed = -150;
					sprite->changeAnimation(CLOSCA_ESQUERRA);
				}
				quieta = false;
			}
		}
		else {
			if (speed > 0 && mapInteractive->collisionMoveRight(posGoomba, glm::ivec2(24, 24))) {
				speed = -speed;
			}
			else if (speed < 0 && mapInteractive->collisionMoveLeft(posGoomba, glm::ivec2(24, 24))) {
				speed = -speed;
			}

			if (mapInteractive->collisionMoveDown(posGoomba, glm::ivec2(24, 24), &posGoomba.y)) {
				speed_y = 0;
			}
			else speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;

			if (speed_y > TERMINAL_VELOCITY) speed_y = TERMINAL_VELOCITY;
			posGoomba.y += speed_y * float(deltaTime) / 1000.f;

			if (killed(posPlayer)) {
				speed = 0;
				sprite->changeAnimation(CLOSCA_QUIETA);
				player->rebota();
				quieta = true;
			}
			else if (collidesWithPlayer(posPlayer)) {
				player->hurt();
			}
		}
		posGoomba.x += speed * float(deltaTime) / 1000.f;
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));

		if (posGoomba.y > 200) {
			alive = false;
		}
	}
}

void CloscaKoopaTroopa::render()
{
	if(alive) sprite->render();
}

void CloscaKoopaTroopa::setTileMapInteractive(TileMapInteractive* tileMapInteractive)
{
	mapInteractive = tileMapInteractive;
}

void CloscaKoopaTroopa::setPosition(const glm::vec2& pos)
{
	posGoomba = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

bool CloscaKoopaTroopa::collidesWithPlayer(glm::ivec2 posPlayer) {
	if (abs(posPlayer.y - posGoomba.y) > 10) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 16) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 24) <= posGoomba.x)) return true;
	return false;
}

bool CloscaKoopaTroopa::killed(glm::ivec2 posPlayer) {
	if ((posPlayer.y + 25) > (posGoomba.y) || (posPlayer.y) < (posGoomba.y - 32)) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 32) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 16) <= posGoomba.x)) return true;
	return false;
}

void CloscaKoopaTroopa::setPlayer(Player* player) {
	if (player != NULL) this->player = player;
}

bool CloscaKoopaTroopa::moving() {
	return !quieta;
}

glm::ivec2 CloscaKoopaTroopa::getPos() {
	return posGoomba;
}