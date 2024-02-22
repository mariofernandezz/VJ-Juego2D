#ifndef _CLOSCAKOOPATROOPA_INCLUDE
#define _CLOSCAKOOPATROOPA_INCLUDE


#include "Sprite.h"
#include "TileMapInteractive.h"
#include "Player.h"


class CloscaKoopaTroopa
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();

	void setTileMapInteractive(TileMapInteractive* tileMapInteractive);
	void setPosition(const glm::vec2& pos);
	void setPlayer(Player* player);
	bool moving();
	glm::ivec2 getPos();

private:
	bool collidesWithPlayer(glm::ivec2 posPlayer);
	bool killed(glm::ivec2 posPlayer);
	float speed, speed_y;
	glm::ivec2 tileMapDispl, posGoomba;
	bool quieta, alive;
	Texture spritesheet;
	Sprite* sprite;
	TileMapInteractive* mapInteractive;
	Player* player;
};


#endif // _PLAYER_INCLUDE


