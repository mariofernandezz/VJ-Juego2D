#include "SceneInteractive.h"
#include <iostream>


SceneInteractive::SceneInteractive() {
	numberSprite = NULL;
	characterSprite = NULL;
}


SceneInteractive::~SceneInteractive() {
	if (numberSprite != NULL)
		delete numberSprite;
	if (characterSprite != NULL)
		delete characterSprite;
}

void SceneInteractive::init(ShaderProgram& shaderProgram, glm::ivec2 playerPos, int level, int score)
{
	initSpritesNum(shaderProgram);
	initSpritesChars(shaderProgram);
	initPause(shaderProgram);

	puntos = score;
	tiempoRestante = 60;
	nivel = level;
	estado = Starting;
	char2id = { {'a',0}, {'c',1}, {'d',2}, {'e',3},
				{'g',4}, {'l',5}, {'m',6}, {'o',7},
				{'r',8}, {'s',9}, {'t',10}, {'v',11},
				{'y',12}, {'?',13}, {'x',14} };
	spritePosAntigua = playerPos;
}

void SceneInteractive::initSpritesNum(ShaderProgram& shaderProgram) {
	numbersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbersSpritesheet.setMagFilter(GL_NEAREST);
	numberSprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram);

	numberSprite->setNumberAnimations(10);

	for (int i = 0; i < 10; ++i)
	{
		numberSprite->setAnimationSpeed(i, 1);
		numberSprite->addKeyframe(i, glm::vec2(i / 10.f, 0.f));
	}
}

void SceneInteractive::initSpritesChars(ShaderProgram& shaderProgram) {
	charactersSpritesheet.loadFromFile("images/chars.png", TEXTURE_PIXEL_FORMAT_RGBA);
	charactersSpritesheet.setMagFilter(GL_NEAREST);
	characterSprite = Sprite::createSprite(glm::ivec2(8, 8), glm::vec2(0.25, 0.25), &charactersSpritesheet, &shaderProgram);

	characterSprite->setNumberAnimations(16);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4;++j)
		{
			int animId = i * 4 + j;
			characterSprite->setAnimationSpeed(animId, 1);
			characterSprite->addKeyframe(animId, glm::vec2(j / 4.f, i / 4.f));
		}
}

void SceneInteractive::initPause(ShaderProgram& shaderProgram) {
	pauseSpriteSheet.loadFromFile("images/p.png", TEXTURE_PIXEL_FORMAT_RGBA);
	pauseSpriteSheet.setMagFilter(GL_NEAREST);
	pauseSprite = Sprite::createSprite(glm::ivec2(99, 24), glm::vec2(1, 1), &pauseSpriteSheet, &shaderProgram);
}

void SceneInteractive::updatePuntos(int s)
{
	puntos = s;
}

void SceneInteractive::updatetiempoRestante(int t)
{
	tiempoRestante = t;
}

void SceneInteractive::updateNivel(int s)
{
	nivel = s;
}

void SceneInteractive::render(glm::ivec2 playerPos) {

	if (playerPos.x > spritePosAntigua.x) {
		renderChar('s', playerPos.x + 50 - 150, 8);
		renderChar('c', playerPos.x + 58 - 150, 8);
		renderChar('o', playerPos.x + 66 - 150, 8);
		renderChar('r', playerPos.x + 74 - 150, 8);
		renderChar('e', playerPos.x + 82 - 150, 8);
		renderNumber(puntos / 100000, playerPos.x + 50 - 150, 16);
		renderNumber((puntos % 100000) / 10000, playerPos.x + 58 - 150, 16);
		renderNumber((puntos % 10000) / 1000, playerPos.x + 66 - 150, 16);
		renderNumber((puntos % 1000) / 100, playerPos.x + 74 - 150, 16);
		renderNumber((puntos % 100) / 10, playerPos.x + 82 - 150, 16);
		renderNumber(puntos % 10, playerPos.x + 90 - 150, 16);

		renderChar('s', playerPos.x + 268 - 150, 16);
		renderChar('t', playerPos.x + 276 - 150, 16);
		renderChar('a', playerPos.x + 284 - 150, 16);
		renderChar('g', playerPos.x + 292 - 150, 16);
		renderChar('e', playerPos.x + 300 - 150, 16);
		// stage
		renderNumber(nivel / 10, playerPos.x + 308 - 150, 16);
		renderNumber(nivel % 10, playerPos.x + 316 - 150, 16);

		renderChar('s', playerPos.x + 170 - 150, 8);
		renderChar('e', playerPos.x + 178 - 150, 8);
		renderChar('c', playerPos.x + 186 - 150, 8);
		renderNumber(tiempoRestante / 10, playerPos.x + 170 - 150, 16);
		renderNumber(tiempoRestante % 10, playerPos.x + 178 - 150, 16);
		
		spritePosAntigua.x = playerPos.x;
	}
	else {
		renderChar('s', spritePosAntigua.x + 50 - 150, 8);
		renderChar('c', spritePosAntigua.x + 58 - 150, 8);
		renderChar('o', spritePosAntigua.x + 66 - 150, 8);
		renderChar('r', spritePosAntigua.x + 74 - 150, 8);
		renderChar('e', spritePosAntigua.x + 82 - 150, 8);
		renderNumber(puntos / 100000, spritePosAntigua.x + 50 - 150, 16);
		renderNumber((puntos % 100000) / 10000, spritePosAntigua.x + 58 - 150, 16);
		renderNumber((puntos % 10000) / 1000, spritePosAntigua.x + 66 - 150, 16);
		renderNumber((puntos % 1000) / 100, spritePosAntigua.x + 74 - 150, 16);
		renderNumber((puntos % 100) / 10, spritePosAntigua.x + 82 - 150, 16);
		renderNumber(puntos % 10, spritePosAntigua.x + 90 - 150, 16);

		renderChar('s', spritePosAntigua.x + 268 - 150, 16);
		renderChar('t', spritePosAntigua.x + 276 - 150, 16);
		renderChar('a', spritePosAntigua.x + 284 - 150, 16);
		renderChar('g', spritePosAntigua.x + 292 - 150, 16);
		renderChar('e', spritePosAntigua.x + 300 - 150, 16);
		// stage
		renderNumber(nivel / 10, spritePosAntigua.x + 308 - 150, 16);
		renderNumber(nivel % 10, spritePosAntigua.x + 316 - 150, 16);

		renderChar('s', spritePosAntigua.x + 170 - 150, 8);
		renderChar('e', spritePosAntigua.x + 178 - 150, 8);
		renderChar('c', spritePosAntigua.x + 186 - 150, 8);
		renderNumber(tiempoRestante / 10, spritePosAntigua.x + 170 - 150, 16);
		renderNumber(tiempoRestante % 10, spritePosAntigua.x + 178 - 150, 16);
	}
	renderMessages(playerPos);
}

void SceneInteractive::renderMessages(glm::ivec2 playerPos)
{
	if (estado == Starting)
	{
		renderChar('r', 160, 100);
		renderChar('e', 168, 100);
		renderChar('a', 176, 100);
		renderChar('d', 184, 100);
		renderChar('y', 192, 100);
		renderChar('?', 200, 100);
	}

	else if (estado == Pause)
	{
		pauseSprite->setPosition(glm::vec2(spritePosAntigua.x, spritePosAntigua.y-50));
		pauseSprite->render();
	}
		
	else if (estado == LevelComplete)
	{
		renderChar('s', playerPos.x + 250 - 225, 105);
		renderChar('t', playerPos.x + 258 - 225, 105);
		renderChar('a', playerPos.x + 266 - 225, 105);
		renderChar('g', playerPos.x + 274 - 225, 105);
		renderChar('e', playerPos.x + 282 - 225, 105);

		renderChar('c', playerPos.x + 298 - 225, 105);
		renderChar('l', playerPos.x + 306 - 225, 105);
		renderChar('e', playerPos.x + 314 - 225, 105);
		renderChar('a', playerPos.x + 322 - 225, 105);
		renderChar('r', playerPos.x + 330 - 225, 105);
	}
	else if (estado == GameOver)
	{
		renderChar('g', spritePosAntigua.x + 250 - 250, 100);
		renderChar('a', spritePosAntigua.x + 258 - 250, 100);
		renderChar('m', spritePosAntigua.x + 266 - 250, 100);
		renderChar('e', spritePosAntigua.x + 274 - 250, 100);

		renderChar('o', spritePosAntigua.x + 290 - 250, 100);
		renderChar('v', spritePosAntigua.x + 298 - 250, 100);
		renderChar('e', spritePosAntigua.x + 306 - 250, 100);
		renderChar('r', spritePosAntigua.x + 314 - 250, 100);
	}
}

void SceneInteractive::renderNumber(int n, int x, int y)
{
	numberSprite->changeAnimation(n);
	numberSprite->setPosition(glm::vec2(x, y));
	numberSprite->render();
}

void SceneInteractive::renderChar(char c, int x, int y)
{
	characterSprite->changeAnimation(char2id[c]);
	characterSprite->setPosition(glm::vec2(x, y));
	characterSprite->render();
}


