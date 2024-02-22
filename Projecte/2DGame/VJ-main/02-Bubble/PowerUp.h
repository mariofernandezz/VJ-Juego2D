#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE


#include "Sprite.h"
#include "TileMapInteractive.h"
#include "player.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class PowerUp
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, int type);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();

	void setTileMapInteractive(TileMapInteractive* tileMapInteractive);
	void setPosition(const glm::vec2& pos);

	void setPlayer(Player* player);



private:
	bool collidesWithPlayer(glm::ivec2 posPlayer);
	bool killed(glm::ivec2 posPlayer);
	glm::ivec2 tileMapDispl, posGoomba;
	bool alive;
	int Type;
	float speed, speed_y;
	float posMaxMoneda;
	Texture spritesheet;
	Sprite* sprite;
	TileMapInteractive* mapInteractive;
	Player* player;
};


#endif // _PLAYER_INCLUDE


