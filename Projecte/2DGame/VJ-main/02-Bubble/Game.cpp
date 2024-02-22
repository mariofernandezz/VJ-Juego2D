#include <GL/glew.h>
#include <GL/glut.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <iostream>


void Game::init()
{
	initShaders();
	bPlay = true;
	glClearColor(97.0/255, 133.0/255, 248.0/255, 1.0);
	
	menu.init(texProgram);
	isMenuShowing = true;
	opciones = 0;
	nivelActual = 1;
}

bool Game::update(int deltaTime)
{
	if (isMenuShowing) menu.update(deltaTime);
	else {
		switch (scene->estado)
		{
		case Scene::Starting:
			scene->update(deltaTime);
			break;
		case Scene::Playing:
			scene->update(deltaTime);
			break;
		case Scene::Pause:
			scene->update(deltaTime);
			break;
		case Scene::LevelComplete:
			if (nivelesCompletados == 0) {
				if (nivelActual == 1) nivelActual = 2;	
				else nivelActual = 1;
				++nivelesCompletados;
				score += scene->getPlayerScore();
				cargaPrimerNivel();
			}
			else
			{
				score += scene->getPlayerScore();
				menu.setPlayerFinalScore(score);
				menu.setMenuState(Menu::Win);
				isMenuShowing = true;
			}
			break;
		case Scene::GameOver:
			menu.setPlayerFinalScore(score);
			menu.setMenuState(Menu::Lose);
			isMenuShowing = true;
			noMenuPrincipal = true;
			break;
		default:
			break;
		}
	}
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelview;

	if (isMenuShowing) {
		texProgram.use();
		texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		texProgram.setUniformMatrix4f("projection", projection);
		modelview = glm::mat4(1.0f);
		texProgram.setUniformMatrix4f("modelview", modelview);
		texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
		projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
		menu.render();
	}
	else scene->render();
}

void Game::keyPressed(int key)
{
	// Escape code 
	if (key == 27 && (!isMenuShowing || menu.getMenuState() != Menu::MenuState::Main)) {
		if (!isMenuShowing || noMenuPrincipal)
		{
			isMenuShowing = true;
			noMenuPrincipal = false;
			menu.setMenuState(Menu::MenuState::Main);
			score = 0;
			nivelActual = 1;
		}
		else bPlay = false;
	}

	// Enter
	if ((key == 13 || key == 32) && isMenuShowing && menu.getMenuState() == Menu::MenuState::Main)
	{
		optionSelected();
	}

	//Creditos "C"
	if ((key == 99 || key == 69 || key == 67) && menu.getMenuState() == Menu::MenuState::Main)
	{
		menu.setMenuState(Menu::MenuState::Credits);
	}
	//Quitar creditos "C" 
	else if ((key == 99 || key == 69 || key == 67) && menu.getMenuState() == Menu::MenuState::Credits) {
		menu.setMenuState(Menu::MenuState::Main);
	}
	// Pulsa G
	if ((key == 103 || key == 71) && !isMenuShowing)
	{
		if (!scene->isMarioEstrella()) {
			scene->setMarioEstrella(true);
			scene->setMarioBros(false);
		}
	}
	// Pulsa M
	if (key == 109 && !isMenuShowing)
	{
		if (!scene->isMarioBros()) {
			scene->setMarioBros(true);
			scene->setMarioEstrella(false);
		}
	}
	//Load stage 1
	if (key == 49 && isMenuShowing)
	{
		nivelActual = 1;
		cargaPrimerNivel();
	}
	//Load stage 2
	if (key == 50 && isMenuShowing)
	{
		nivelActual = 2;
		cargaPrimerNivel();
	}
	// Pausa
	if (key == 112 && !isMenuShowing)
	{
		char state = scene->getState();
		if (state == 'a')
		{
			scene->setState('p');
		}
		else if (state == 'p')
		{
			scene->setState('a');
			scene->setSceneInterfaceState('p');
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	if (key == 100 && isMenuShowing && menu.getMenuState() == Menu::MenuState::Main)
	{
		--opciones;
		if (opciones < 0) opciones = 3;
		menu.setSelection(opciones);
	}
	if (key == 102 && isMenuShowing && menu.getMenuState() == Menu::MenuState::Main)
	{
		++opciones;
		if (opciones > 3) opciones = 0;
		menu.setSelection(opciones);
	}
	specialKeys[key] = true;

}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}

void Game::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}


void Game::optionSelected()
{
	// Empieza a jugar en el nivel 1
	if (opciones == 0) {
		cargaPrimerNivel();
	}
	// Entra en el menu de help
	if (opciones == 1) {
		noMenuPrincipal = true;
		menu.setMenuState(Menu::MenuState::Help);
	}
	// Entra en el menu de credits
	if (opciones == 2) {
		noMenuPrincipal = true;
		menu.setMenuState(Menu::MenuState::Credits);
	}
	// Sale del juego
	if (opciones == 3) {
		bPlay = false;
	}
}

void Game::cargaPrimerNivel()
{
	scene = new Scene();
	scene->init(texProgram, nivelActual, score);

	isMenuShowing = false;
}






