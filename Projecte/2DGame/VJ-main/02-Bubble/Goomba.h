#ifndef _GOOMBA_INCLUDE
#define _GOOMBA_INCLUDE


#include "Sprite.h"
#include "TileMapInteractive.h"
#include "player.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Goomba
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();

	void setTileMapInteractive(TileMapInteractive* tileMapInteractive);
	void setPosition(const glm::vec2& pos);

	void setPlayer(Player* player);

	glm::ivec2 getPos();
	void dies();
	bool isAlive();

private:
	bool collidesWithPlayer(glm::ivec2 posPlayer);
	bool killed(glm::ivec2 posPlayer);
	glm::ivec2 tileMapDispl, posGoomba;
	bool alive, despert;
	float speed, speed_y;
	Texture spritesheet;
	Sprite* sprite;
	TileMapInteractive* mapInteractive;
	Player* player;
	float dyingTime;
};


#endif // _PLAYER_INCLUDE


