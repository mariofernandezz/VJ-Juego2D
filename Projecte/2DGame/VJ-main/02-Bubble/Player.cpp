#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "TileMapInteractive.h"


#define FALL_STEP 4

#define JUMP -180

#define REBOTA -100

#define VELOCITY_MAX 150
#define SPRINT 50
#define ACCELERATION 50
#define VELOCITY_MIN 70
#define GRAVITY 350
#define TERMINAL_VELOCITY 300
#define FACTOR_IGUALADOR 0.5




enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, JUMP_LEFT, JUMP_RIGHT, MOVE_LEFT, MOVE_RIGHT, 
	STAND_LEFT_SMALL, STAND_RIGHT_SMALL, JUMP_LEFT_SMALL, JUMP_RIGHT_SMALL, MOVE_LEFT_SMALL, MOVE_RIGHT_SMALL,
	STAND_LEFT_E, STAND_RIGHT_E, JUMP_LEFT_E, JUMP_RIGHT_E, MOVE_LEFT_E, MOVE_RIGHT_E,
	DEAD
};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/marios.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 32), glm::vec2(0.0625, 0.125), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(19);
	

	// Mario Grande
	sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.8125f, 0.25f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.875f, 0.25f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.5f, 0.25f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.1875f, 0.375f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.75f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6875f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.625f, 0.25f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.6875f, 0.25f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.9375f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.375f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0625f, 0.375f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.375f));


	// Mario Small
	sprite->setAnimationSpeed(STAND_LEFT_SMALL, 8);
	sprite->addKeyframe(STAND_LEFT_SMALL, glm::vec2(0.375f, 0.f));

	sprite->setAnimationSpeed(STAND_RIGHT_SMALL, 8);
	sprite->addKeyframe(STAND_RIGHT_SMALL, glm::vec2(0.4375f, 0.f));

	sprite->setAnimationSpeed(JUMP_LEFT_SMALL, 8);
	sprite->addKeyframe(JUMP_LEFT_SMALL, glm::vec2(0.0625f, 0.f));

	sprite->setAnimationSpeed(JUMP_RIGHT_SMALL, 8);
	sprite->addKeyframe(JUMP_RIGHT_SMALL, glm::vec2(0.75f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT_SMALL, 8);
	sprite->addKeyframe(MOVE_LEFT_SMALL, glm::vec2(0.3125f, 0.f));
	sprite->addKeyframe(MOVE_LEFT_SMALL, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT_SMALL, glm::vec2(0.1875f, 0.f));
	sprite->addKeyframe(MOVE_LEFT_SMALL, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT_SMALL, 8);
	sprite->addKeyframe(MOVE_RIGHT_SMALL, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT_SMALL, glm::vec2(0.5625f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT_SMALL, glm::vec2(0.625f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT_SMALL, glm::vec2(0.5625f * 8, 0.f));

	// Mario estrella
	sprite->setAnimationSpeed(STAND_LEFT_E, 8);
	sprite->addKeyframe(STAND_LEFT_E, glm::vec2(0.9375f, 0.5f));
	sprite->addKeyframe(STAND_LEFT_E, glm::vec2(0.f, 0.625f));
	sprite->addKeyframe(STAND_LEFT_E, glm::vec2(0.0625f, 0.625f));

	sprite->setAnimationSpeed(STAND_RIGHT_E, 8);
	sprite->addKeyframe(STAND_RIGHT_E, glm::vec2(0.125f, 0.625f));
	sprite->addKeyframe(STAND_RIGHT_E, glm::vec2(0.1875f, 0.625f));
	sprite->addKeyframe(STAND_RIGHT_E, glm::vec2(0.25f, 0.625f));

	sprite->setAnimationSpeed(JUMP_LEFT_E, 8);
	sprite->addKeyframe(JUMP_LEFT_E, glm::vec2(0.375f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT_E, glm::vec2(0.4375f, 0.5f));
	sprite->addKeyframe(JUMP_LEFT_E, glm::vec2(0.5f, 0.5f));

	sprite->setAnimationSpeed(JUMP_RIGHT_E, 8);
	sprite->addKeyframe(JUMP_RIGHT_E, glm::vec2(0.6875f, 0.625f));
	sprite->addKeyframe(JUMP_RIGHT_E, glm::vec2(0.75f, 0.625f));
	sprite->addKeyframe(JUMP_RIGHT_E, glm::vec2(0.8125f, 0.625f));


	sprite->setAnimationSpeed(MOVE_LEFT_E, 8);
	sprite->addKeyframe(MOVE_LEFT_E, glm::vec2(0.75f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT_E, glm::vec2(0.8125f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT_E, glm::vec2(0.875f, 0.5f));
	sprite->addKeyframe(MOVE_LEFT_E, glm::vec2(0.8125f, 0.5f));

	sprite->setAnimationSpeed(MOVE_RIGHT_E, 8);
	sprite->addKeyframe(MOVE_RIGHT_E, glm::vec2(0.3125f, 0.625f));
	sprite->addKeyframe(MOVE_RIGHT_E, glm::vec2(0.375f, 0.625f));
	sprite->addKeyframe(MOVE_RIGHT_E, glm::vec2(0.4375f, 0.625f));
	sprite->addKeyframe(MOVE_RIGHT_E, glm::vec2(0.375f, 0.625f));

	//Muerte
	sprite->setAnimationSpeed(DEAD, 8);
	sprite->addKeyframe(DEAD, glm::vec2(0.f, 0.f));

		
	sprite->changeAnimation(STAND_RIGHT_SMALL);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	

	speed_x = 0;
	speed_y = 0;
	spacePressed = false;
	invencible = 0;
}

void Player::update(int deltaTime, float posCamera)
{
	sprite->update(deltaTime);

	if (sprite->animation() != DEAD && !animDead) {
		if (downgrade) {
			if(sprite->animation() == STAND_LEFT) sprite->changeAnimation(STAND_LEFT_SMALL);
			else if (sprite->animation() == STAND_RIGHT) sprite->changeAnimation(STAND_RIGHT_SMALL);
			else if (sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(JUMP_RIGHT_SMALL);
			else if (sprite->animation() == JUMP_LEFT) sprite->changeAnimation(JUMP_LEFT_SMALL);
			else if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT_SMALL);
			else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT_SMALL);

		}
		else if (bEstrella && sprite->animation() != STAND_LEFT_E && sprite->animation() != STAND_RIGHT_E) {
			if (sprite->animation() == STAND_LEFT_SMALL || sprite->animation() == STAND_LEFT)
				sprite->changeAnimation(STAND_LEFT_E);
			else if (sprite->animation() == STAND_RIGHT_SMALL || sprite->animation() == STAND_RIGHT)
				sprite->changeAnimation(STAND_RIGHT_E);
		}
		else if (bBros) {
			if (sprite->animation() == STAND_LEFT_SMALL || sprite->animation() == STAND_LEFT_E)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == STAND_RIGHT_SMALL || sprite->animation() == STAND_RIGHT_E)
				sprite->changeAnimation(STAND_RIGHT);
		}
		else if (!bEstrella && StartoBros) {
			StartoBros = false;
			if (sprite->animation() == STAND_LEFT_E) sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == STAND_RIGHT_E) sprite->changeAnimation(STAND_RIGHT);
		}
		// Clica izquierda
		if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
			facingRight = false;
			if (sprite->animation() != MOVE_LEFT_SMALL && !bEstrella && !bBros)
				sprite->changeAnimation(MOVE_LEFT_SMALL);
			else if (sprite->animation() != MOVE_LEFT_E && bEstrella && !bBros)
				sprite->changeAnimation(MOVE_LEFT_E);
			else if (sprite->animation() != MOVE_LEFT && !bEstrella && bBros)
				sprite->changeAnimation(MOVE_LEFT);

			if (mapInteractive->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)) || posPlayer.x < posCamera - 135)
			{
				if (!bEstrella && !bBros)
					sprite->changeAnimation(STAND_LEFT_SMALL);
				else if (!bEstrella && bBros)
					sprite->changeAnimation(STAND_LEFT);
				else if (bEstrella && bBros)
					sprite->changeAnimation(STAND_LEFT_E);
				speed_x = 0;
			}
			else {
				speed_x -= ACCELERATION * float(deltaTime) / 1000.f;
				if (speed_x < -VELOCITY_MAX) speed_x = -VELOCITY_MAX;
				else if (speed_x > -VELOCITY_MIN) speed_x = -VELOCITY_MIN;
				float speed_x_shift = speed_x * FACTOR_IGUALADOR;
				if (Game::instance().getSpecialKey(112)) speed_x_shift -= SPRINT;
				posPlayer.x += speed_x_shift * float(deltaTime) / 1000.f;
			}
		}
		else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
			facingRight = true;
			if (sprite->animation() != MOVE_RIGHT_SMALL && !bEstrella && !bBros)
				sprite->changeAnimation(MOVE_RIGHT_SMALL);
			else if (sprite->animation() != MOVE_RIGHT_E && bEstrella && !bBros)
				sprite->changeAnimation(MOVE_RIGHT_E);
			else if (sprite->animation() != MOVE_RIGHT && !bEstrella && bBros)
				sprite->changeAnimation(MOVE_RIGHT);
			if (mapInteractive->collisionMoveRight(posPlayer, glm::ivec2(24, 32)))
			{
				if (!bEstrella && !bBros)
					sprite->changeAnimation(STAND_RIGHT_SMALL);
				else if (!bEstrella && bBros)
					sprite->changeAnimation(STAND_RIGHT);
				else if (bEstrella && bBros)
					sprite->changeAnimation(STAND_RIGHT_E);
				speed_x = 0;
			}
			else {
				speed_x += ACCELERATION * float(deltaTime) / 1000.f;
				if (speed_x > VELOCITY_MAX) speed_x = VELOCITY_MAX;
				else if (speed_x < VELOCITY_MIN) speed_x = VELOCITY_MIN;
				float speed_x_shift = speed_x;
				if (Game::instance().getSpecialKey(112)) {
					speed_x_shift += SPRINT;
				}
				posPlayer.x += speed_x_shift * float(deltaTime) / 1000.f;
			}
		}
		else
		{
			speed_x = 0;
			if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
			else if (sprite->animation() == MOVE_LEFT_E) sprite->changeAnimation(STAND_LEFT_E);
			else if (sprite->animation() == MOVE_RIGHT_E) sprite->changeAnimation(STAND_RIGHT_E);
			else if (sprite->animation() == MOVE_LEFT_SMALL) sprite->changeAnimation(STAND_LEFT_SMALL);
			else if (sprite->animation() == MOVE_RIGHT_SMALL) sprite->changeAnimation(STAND_RIGHT_SMALL);
		}

		// posY
		if (bJumping)
		{
			spacePressed = spacePressed && Game::instance().getKey(32);
			if (!facingRight && (sprite->animation() != JUMP_LEFT && sprite->animation() != JUMP_LEFT_SMALL && sprite->animation() != JUMP_LEFT_E)) {
				if (!bEstrella && !bBros) sprite->changeAnimation(JUMP_LEFT_SMALL);
				else if (bEstrella && !bBros && sprite->animation() != JUMP_LEFT_E) sprite->changeAnimation(JUMP_LEFT_E);
				else if (!bEstrella && bBros) sprite->changeAnimation(JUMP_LEFT);
			}
			else if (facingRight && (sprite->animation() != JUMP_RIGHT && sprite->animation() != JUMP_RIGHT_SMALL && sprite->animation() != JUMP_RIGHT_E)) {
				if (!bEstrella && !bBros) sprite->changeAnimation(JUMP_RIGHT_SMALL);
				else if (bEstrella && !bBros && sprite->animation() != JUMP_RIGHT_E) sprite->changeAnimation(JUMP_RIGHT_E);
				else if (!bEstrella && bBros) sprite->changeAnimation(JUMP_RIGHT);
			}
			if (spacePressed && speed_y < 0) speed_y += GRAVITY * float(deltaTime) / 1000.f;
			else {
				speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;
			}

		}
		else
		{
			//posPlayer.y += FALL_STEP;
			if (mapInteractive->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y))
			{
				if (facingRight && sprite->animation() == JUMP_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				else if (!facingRight && sprite->animation() == JUMP_LEFT) sprite->changeAnimation(STAND_LEFT);
				else if (facingRight && sprite->animation() == JUMP_RIGHT_E) sprite->changeAnimation(STAND_RIGHT_E);
				else if (!facingRight && sprite->animation() == JUMP_LEFT_E) sprite->changeAnimation(STAND_LEFT_E);
				else if (facingRight && sprite->animation() == JUMP_RIGHT_SMALL) sprite->changeAnimation(STAND_RIGHT_SMALL);
				else if (!facingRight && sprite->animation() == JUMP_LEFT_SMALL) sprite->changeAnimation(STAND_LEFT_SMALL);

				if (Game::instance().getKey(32))
				{
					if (!spacePressed) {
						bJumping = true;
						spacePressed = true;
						speed_y = JUMP;
					}
				}
				else spacePressed = false;
			}
			else {
				speed_y += GRAVITY * 2 * float(deltaTime) / 1000.f;
			}
		}

		if (((bEstrella || bBros) && mapInteractive->collisionMoveUp(posPlayer, glm::ivec2(24, 32), &posPlayer.y, true)) || mapInteractive->collisionMoveUp(glm::ivec2(posPlayer.x, posPlayer.y + 16), glm::ivec2(16, 16), &posPlayer.y, true)) {
			if (speed_y < 0) speed_y = 0;
		}
		else if (mapInteractive->collisionMoveDown(posPlayer, glm::ivec2(24, 32), &posPlayer.y) && speed_y >= 0)
		{
			bJumping = false;
			speed_y = 0;
		}
		if (speed_y > TERMINAL_VELOCITY) speed_y = TERMINAL_VELOCITY;
		posPlayer.y += speed_y * float(deltaTime) / 1000.f;
	}
	else if (sprite->animation() == DEAD && animDead){
		posPlayer.y += -12;
		animDead = true;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMapInteractive* tileMapInteractive)
{
	mapInteractive = tileMapInteractive;
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::rebota() {
	if (Game::instance().getKey(32)) {
		speed_y = JUMP;
		spacePressed = true;
	}
	else speed_y = REBOTA;
	bJumping = true;
}

void Player::hurt() {
	//	quan un enemic fereix al mario crida aquesta funcio
	if (invencible == 0) {
		if (bBros && !bEstrella) {
			downgrade = true;
			//immunity = 20;
			bBros = false;
			invencible = 3;
		}
		else if (!bBros && !bEstrella) {
			isDead = true;
			sprite->changeAnimation(DEAD);
			posPlayer.y += -15;
		}
	}
}
glm::ivec2 Player::getPosition()
{
	return posPlayer;
}

void Player::addScore(int s)
{
	score += s;
}

void Player::powerUp(int type) {
	// 1 = coin, 2 = super, 3 = star
	if (type == 2) {
		score += 100;
		if (!bBros && !bEstrella) bBros = true;
	}
	else if (type == 3) {
		bEstrella = true;
		bBros = false;
		StartoBros = true;
		tiempoEstrella = 10;
		score += 1000;
	}
}

void Player::getCoin() {
	score += 100;
}

void Player::enemyKilled() {
	score += 150;
}
void Player::lessTimeInvincible() {
	--invencible;
}

void Player::lessTimeEstrella() {
	--tiempoEstrella;
}

void Player::setEstrella(bool b) {
	bEstrella = b; 
	StartoBros = b;
	if(b) tiempoEstrella = 10;
}

/*bool Player::esMini() {
	return (!bBros && !bEstrella);
}*/

