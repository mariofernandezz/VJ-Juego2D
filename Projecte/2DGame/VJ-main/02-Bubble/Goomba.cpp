#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Goomba.h"
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



enum GoombaAnims
{
	MOVE_LEFT, MOVE_RIGHT, DEAD
};


void Goomba::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{

	spritesheet.loadFromFile("images/Goomba2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24,24), glm::vec2(0.20, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(3);

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.75f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.75f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.75f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6f, 0.75f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.8f, 0.75f));


	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.6, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.8, 0.25f));

	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.2, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.4, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.6, 0.f));
	sprite->addKeyframe(DEAD, glm::vec2(0.8, 0.f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
	sprite->changeAnimation(MOVE_LEFT);
	speed = -100;
	speed_y = 0;
	alive = true;
	despert = false;
	dyingTime = 600.f;
}

void Goomba::update(int deltaTime, glm::ivec2 posPlayer)
{
	if (abs(posPlayer.x - posGoomba.x) < 200) despert = true;
	if (despert) {
		if (alive) {
			sprite->update(deltaTime);
			if (speed > 0 && mapInteractive->collisionMoveRight(posGoomba, glm::ivec2(24, 24))) {
				sprite->changeAnimation(MOVE_LEFT);
				speed = -100 * FACTOR_IGUALADOR;
			}
			else if (speed < 0 && mapInteractive->collisionMoveLeft(posGoomba, glm::ivec2(24, 24))) {
				sprite->changeAnimation(MOVE_RIGHT);
				speed = 100;
			}
			posGoomba.x += speed * deltaTime / 1000.f;


			if (killed(posPlayer)) {
				alive = false;
				player->rebota();
				sprite->changeAnimation(DEAD);
				player->enemyKilled();
			}
			else if (collidesWithPlayer(posPlayer)) {
				player->hurt();
			}
			
			if (mapInteractive->collisionMoveDown(posGoomba, glm::ivec2(24, 24), &posGoomba.y)) {
				speed_y = 0;
			}
			else speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;

			if (speed_y > TERMINAL_VELOCITY) speed_y = TERMINAL_VELOCITY;
			posGoomba.y += speed_y * float(deltaTime) / 1000.f;
			
			sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
		}
		else if (dyingTime > 0) {
			sprite->update(deltaTime);
			dyingTime -= deltaTime;
		}
		if (posGoomba.y > 200) {
			alive = false;
			dyingTime = 0;
		}
	}

}

void Goomba::render()
{ 
	if(dyingTime>0) sprite->render();
}

void Goomba::setTileMapInteractive(TileMapInteractive* tileMapInteractive)
{
	mapInteractive = tileMapInteractive;
}

void Goomba::setPosition(const glm::vec2& pos)
{
	posGoomba = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGoomba.x), float(tileMapDispl.y + posGoomba.y)));
}

bool Goomba::collidesWithPlayer(glm::ivec2 posPlayer) {
	if (abs(posPlayer.y - posGoomba.y) > 10) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 16) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 24) <= posGoomba.x)) return true;
	return false;
}


bool Goomba::killed(glm::ivec2 posPlayer) {
	if (posPlayer.y > (posGoomba.y - 17) || posPlayer.y < (posGoomba.y - 24)) return false;
	if ((posPlayer.x <= posGoomba.x) && ((posPlayer.x + 24) >= posGoomba.x)) return true;
	if ((posPlayer.x >= posGoomba.x) && ((posPlayer.x - 24) <= posGoomba.x)) return true;
	else return false;
	
}

void Goomba::setPlayer(Player* player) {
	this->player = player;
}

glm::ivec2 Goomba::getPos() {
	return posGoomba;
}
void Goomba::dies() {
	alive = false;
	sprite->changeAnimation(DEAD);
}

bool Goomba::isAlive() {
	return alive;
}