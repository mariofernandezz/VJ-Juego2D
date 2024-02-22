#ifndef _MENU_INCLUDE
#define _MENU_INCLUDE

#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"


class Menu
{

public:
	Menu();
	~Menu();
	enum MenuState {Main, Help, Credits, Win, Lose};
	MenuState estado = Main;

	void init(ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();
	void setMenuState(MenuState m) {estado = m; };
	void setPlayerFinalScore(int i) { playerFinalScore += i; };

	MenuState getMenuState() { return estado; };
	void setSelection(int i);

	void initNumberSprite(ShaderProgram& shaderProgram);
	void renderNumber(int n, int x, int y);
	void renderScore();


private:
	ShaderProgram texProgram;
	Texture mainMenuSheet, helpSpriteSheet, creditsSpriteSheet, winSpriteSheet, loseSpriteSheet, numbersSpritesheet;;
	Sprite* mainMenu;
	Sprite* helpSprite;
	Sprite* creditsSprite;
	Sprite* winSprite;
	Sprite* loseSprite;
	Sprite* numberSprite;

	int playerFinalScore = 0;
};

#endif // _MENU_INCLUDE
