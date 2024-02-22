#ifndef _SCENEINTERACTIVE_INCLUDE
#define _SCENEINTERACTIVE_INCLUDE
#include "Sprite.h"
#include <map>



class SceneInteractive
{
public:
	SceneInteractive();
	~SceneInteractive();

	void init(ShaderProgram& shaderProgram, glm::ivec2 playerPos, int level, int score);
	void render(glm::ivec2 playerPos);

	void renderMessages(glm::ivec2 playerPos);
	void renderChar(char c, int x, int y);
	void renderNumber(int n, int x, int y);

	enum gamestate { Starting, Playing, Pause, GameOver, LevelComplete };
	void setState(gamestate s) { estado = s; };
	void setDeltaTime(int deltatime) { msTime += deltatime; };

	void updatePuntos(int s);
	void updatetiempoRestante(int t);
	void updateNivel(int s);

private:
	void initSpritesNum(ShaderProgram& shaderProgram);
	void initSpritesChars(ShaderProgram& shaderProgram);
	void initPause(ShaderProgram& shaderProgram);


private:
	Texture numbersSpritesheet, charactersSpritesheet, heartSpritesheet, pauseSpriteSheet, lifeUpSpriteSheet;

	// TENEMOS UN SOLO SPRITE PARA CADA COSA Y LO VAMOS COPIANDO A LOS LADOS E IMPREMINEDO VARIAS VECES EN CADA UPDATE

	Sprite* numberSprite;
	Sprite* characterSprite;
	Sprite* pauseSprite;

	map<char, int> char2id;
	int msTime = 0;

	int puntos, tiempoRestante, nivel;
	
	gamestate estado;
	glm::ivec2 spritePosAntigua;
};

#endif

