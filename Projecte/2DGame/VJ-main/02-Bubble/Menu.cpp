#include "Menu.h"
#include "Game.h"
#include <iostream>


Menu::Menu()
{
	mainMenu = NULL;
}

Menu::~Menu()
{
	
}

void Menu::init(ShaderProgram& shaderProgram)
{
	texProgram = shaderProgram;
	estado = Main;

	//Cargar imagen del sprite del menu
	mainMenuSheet.loadFromFile("images/MENUS.png", TEXTURE_PIXEL_FORMAT_RGBA);
	mainMenuSheet.setMagFilter(GL_NEAREST);
	mainMenu = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(0.5f, 0.5f), &mainMenuSheet, &shaderProgram);

	//Cargar animaciones de seleccion de opciones
	mainMenu->setNumberAnimations(4);
	mainMenu->setAnimationSpeed(0, 1);
	mainMenu->addKeyframe(0, glm::vec2(0.f, 0.f));
	mainMenu->setAnimationSpeed(1, 1);
	mainMenu->addKeyframe(1, glm::vec2(0.5f, 0.f));
	mainMenu->setAnimationSpeed(2, 1);
	mainMenu->addKeyframe(2, glm::vec2(0.f, 0.5f));
	mainMenu->setAnimationSpeed(3, 1);
	mainMenu->addKeyframe(3, glm::vec2(0.5f, 0.5f));

	mainMenu->changeAnimation(0);
	mainMenu->setPosition(glm::vec2(0.f, 0.f));

	helpSpriteSheet.loadFromFile("images/hel.png", TEXTURE_PIXEL_FORMAT_RGBA);
	helpSpriteSheet.setMagFilter(GL_NEAREST);
	helpSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &helpSpriteSheet, &shaderProgram);
	helpSprite->setPosition(glm::vec2(0.f, 0.f));

	creditsSpriteSheet.loadFromFile("images/cred.png", TEXTURE_PIXEL_FORMAT_RGBA);
	creditsSpriteSheet.setMagFilter(GL_NEAREST);
	creditsSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &creditsSpriteSheet, &shaderProgram);
	creditsSprite->setPosition(glm::vec2(0.f, 0.f));

	winSpriteSheet.loadFromFile("images/win.png", TEXTURE_PIXEL_FORMAT_RGBA);
	winSpriteSheet.setMagFilter(GL_NEAREST);
	winSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &winSpriteSheet, &shaderProgram);
	winSprite->setPosition(glm::vec2(0.f, 0.f));

	loseSpriteSheet.loadFromFile("images/lose.png", TEXTURE_PIXEL_FORMAT_RGBA);
	loseSpriteSheet.setMagFilter(GL_NEAREST);
	loseSprite = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &loseSpriteSheet, &shaderProgram);
	loseSprite->setPosition(glm::vec2(0.f, 0.f));

	initNumberSprite(shaderProgram);

}

void Menu::update(int deltaTime)
{
	switch (estado)
	{
	case Main:
		mainMenu->update(deltaTime);
		break;
	case Help:
		helpSprite->update(deltaTime);
		break;
	case Credits:
		creditsSprite->update(deltaTime);
		break;
	case Win:
		winSprite->update(deltaTime);
		break;
	case Lose:
		loseSprite->update(deltaTime);
		break;
	default:
		break;
	}
}

void Menu::render()
{
	switch (estado)
	{
	case Main:
		mainMenu->render();
		break;
	case Help:
		helpSprite->render();
		break;
	case Credits:
		creditsSprite->render();
		break;
	case Win:
		winSprite->render();
		renderScore();
		break;
	case Lose:
		loseSprite->render();
		renderScore();
		break;
	default:
		break;
	}
}

void Menu::setSelection(int i)
{
	mainMenu->changeAnimation(i);
}

void Menu::initNumberSprite(ShaderProgram& shaderProgram)
{
	numbersSpritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	numbersSpritesheet.setMagFilter(GL_NEAREST);
	numberSprite = Sprite::createSprite(glm::ivec2(24, 24), glm::vec2(0.1, 1), &numbersSpritesheet, &shaderProgram);

	numberSprite->setNumberAnimations(10);

	for (int i = 0; i < 10; ++i)
	{
		numberSprite->setAnimationSpeed(i, 1);
		numberSprite->addKeyframe(i, glm::vec2(i / 10.f, 0.f));
	}
}

void Menu::renderNumber(int n, int x, int y)
{
	numberSprite->changeAnimation(n);
	numberSprite->setPosition(glm::vec2(x, y));
	numberSprite->render();
}

void Menu::renderScore()
{
	renderNumber(playerFinalScore / 100000, 372, 425);
	renderNumber((playerFinalScore % 100000) / 10000, 396, 425);
	renderNumber((playerFinalScore % 10000) / 1000, 420, 425);
	renderNumber((playerFinalScore % 1000) / 100, 444, 425);
	renderNumber((playerFinalScore % 100) / 10, 468, 425);
	renderNumber(playerFinalScore % 10, 492, 425);
}
