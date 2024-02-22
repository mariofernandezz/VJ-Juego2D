#ifndef _KOOPATROOPA_INCLUDE
#define _KOOPATROOPA_INCLUDE


#include "Sprite.h"
#include "TileMapInteractive.h"
#include "CloscaKoopaTroopa.h"
#include "Player.h"



class KoopaTroopa
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::ivec2 posPlayer);
	void render();

	void setTileMapInteractive(TileMapInteractive* tileMapInteractive);
	void setPosition(const glm::vec2& pos);

	void setPlayer(Player* player);
	bool isCloscaMooving();
	glm::ivec2 getPos();

private:
	bool collidesWithPlayer(glm::ivec2 posPlayer);
	bool killed(glm::ivec2 posPlayer);
	glm::ivec2 tileMapDispl, posGoomba;
	bool alive, despert;
	float speed, speed_y;
	Texture spritesheet;
	Sprite* sprite;
	TileMapInteractive* mapInteractive;
	CloscaKoopaTroopa* closca;
	Player* player;
};


#endif // _PLAYER_INCLUDE


