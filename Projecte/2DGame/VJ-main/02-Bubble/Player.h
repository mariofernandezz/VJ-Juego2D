#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMapInteractive.h"
#include "TileMap.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, float posCamera);
	void render();
	
	void setPosition(const glm::vec2 &pos);
	void setTileMap(TileMapInteractive* tileMapInteractive);
	void powerUp(int type);

	bool getAnimDead() { return animDead; };
	void setAnimDead() { animDead = false; };

	int getScore() const { return score; };
	void addScore(int s);

	void setEstrella(bool es);
	void setBros(bool br) { bBros = br; };
	bool isPlayerDead() { return isDead; };
	bool isPlayerDowngrade() { return downgrade; };
	void setDowngrade(bool b) { downgrade = b; };
	int isInvencible() { return invencible; };
	int timeEstrella() { return tiempoEstrella; };

	bool isEstrella() { return bEstrella; };
	void getCoin();
	void lessTimeInvincible();
	void lessTimeEstrella();


	glm::ivec2 getPosition();
	float speed_x, speed_y;
	void rebota();
	void hurt();
	//bool esMini();
	void enemyKilled();

private:
	bool bJumping, spacePressed;
	glm::ivec2 tileMapDispl, posPlayer;

	int jumpAngle, startY;
	Texture spritesheet;
	Sprite *sprite;
	TileMapInteractive* mapInteractive;

	bool bEstrella = false;
	bool bBros = false;
	bool facingRight = true;

	bool isDead = false;
	bool animDead = false;
	bool StartoBros = false;

	bool downgrade = false;

	int animacion, endCounter = 0;
	int score = 0;
	int invencible = 0;
	int tiempoEstrella = 0;
};


#endif // _PLAYER_INCLUDE


