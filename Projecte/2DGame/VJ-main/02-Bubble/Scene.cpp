#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"
#include "Camara.h"


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 9
#define INIT_PLAYER_Y_TILES 9

#define MAX_X 100
#define SCREEN_WIDTH 299
#define SCREEN_HEIGHT 224

//enum objetosMortales {};

Scene::Scene()
{
	sceneIn = NULL;
	fondo = NULL;
	mapInteractive = NULL;
	player = NULL;
}

Scene::~Scene()
{
	if (sceneIn != NULL)
		delete sceneIn;
	if(fondo != NULL)
		delete fondo;
	if (mapInteractive != NULL)
		delete mapInteractive;
	if(player != NULL);
		delete player;
	if (goomba != NULL)
		delete goomba;
	if (koopaTroopa != NULL)
		delete koopaTroopa;
}


void Scene::init(ShaderProgram& shaderProgram, int nivel, int puntos)
{
	texProgram = shaderProgram;
	//initShaders();

	//Creacion de la escena
	if (nivel == 1) {
		fondo = TileMap::createTileMap("levels/Fons01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapInteractive = TileMapInteractive::createTileMapInteractive("levels/level01.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}
	else if (nivel == 2) {
		fondo = TileMap::createTileMap("levels/Fons02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
		mapInteractive = TileMapInteractive::createTileMapInteractive("levels/level02.txt", glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
	}


	levelActual = nivel;
	score = puntos;
	initPlayer();
	initEnemics(1);//la gracia seria passarli el nivell en q estem i que posi diferents coses en funcio

	//Creacion de la escena interactiva
	sceneIn = new SceneInteractive();
	sceneIn->init(texProgram, player->getPosition(), levelActual, score);
	
	estado = Starting;
	tiempoMensaje = 30;
	timer = 0;

	isPlayerDead = false;
	isMarioStar = false;
	bMarioBros = false;
	isStarting = true;

	currentTime = 0.0f;
	camara = new Camara(player->getPosition(), texProgram);	
}

void Scene::initShaders()
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

void Scene::update(int deltaTime)
{
	if (estado == Pause) sceneIn->setState(SceneInteractive::gamestate::Pause);
	else {
		currentTime += deltaTime;
		sceneIn->setDeltaTime(deltaTime);
		//Empezando el juego
		if (isStarting)
		{
			if (tiempoMensaje == 0) {
				readyMessage();
			}
			else {
				if (currentTime / 100 != timer) {
					timer = currentTime / 100;
					--tiempoMensaje;
				}
				if (!done) {
					updateEnemies(deltaTime);
					camara->update(player->getPosition(), texProgram);
					done = true;
				}
			}
			updateSceneInterface(deltaTime);
		}
		//Muere el Mario 
		else if (player->isPlayerDead() || isPlayerDead || player->getPosition().y > 170) {
			sceneIn->setState(SceneInteractive::gamestate::GameOver);
			if (timerMuerte == 0) {
				estado = GameOver;
				done = false;
			}
			else {
				if (currentTime / 100 != timer) {
					timer = currentTime / 100;
					--timerMuerte;
					if (!done) {
						player->setAnimDead();
						updatePlayer(deltaTime);
						done = true;
					}
				}
			}
			updateSceneInterface(deltaTime);
		}
		//Llega a la bandera
		else if (player->getPosition().x > 3154) {
			if (tiempoSobrante == 0)
			{
				estado = LevelComplete;
				done = false;
			}
			else
			{
				if (currentTime / 100 != timer)
				{
					timer = currentTime / 100;
					--tiempoSobrante;
					player->addScore(10);
					sceneIn->setState(SceneInteractive::gamestate::LevelComplete);
					
				}
			}
			updateSceneInterface(deltaTime);
		}
		// Pasa de SUper Mario a miniMario
		else if (player->isPlayerDowngrade()) {
			if (player->isInvencible() != 0) {
				if (currentTime / 1000 != timerD) {
					timerD = currentTime / 1000;
					player->lessTimeInvincible();
				}
				else {
					if (player->getPosition().x < 3154) {
						updatePlayer(deltaTime);
						updateEnemies(deltaTime);
						updateItems(deltaTime);
						updateTime(deltaTime);
						collisionCloscaGoomba();
						camara->update(player->getPosition(), texProgram);
					}
					updateSceneInterface(deltaTime);
				}
			}
			else {
				player->setDowngrade(false);
				bMarioBros = false;
			}
		}
		//Tiempo para Mario estrella
		else if(player->isEstrella()) {
			if (player->timeEstrella() != 0) {
				if (currentTime / 1000 != timerD) {
					timerD = currentTime / 1000;
					player->lessTimeEstrella();
				}
				else {
					updatePlayer(deltaTime);
					updateEnemies(deltaTime);
					updateItems(deltaTime);
					updateSceneInterface(deltaTime);
					updateTime(deltaTime);
					collisionCloscaGoomba();
					camara->update(player->getPosition(), texProgram);
				}
			}
			else {
				player->setEstrella(false);
				player->setBros(true);
				isMarioStar = false;
				updatePlayer(deltaTime);
			}
		}
		else {
			updatePlayer(deltaTime);
			updateEnemies(deltaTime);
			updateItems(deltaTime);
			updateSceneInterface(deltaTime);

			collisionCloscaGoomba();
			camara->update(player->getPosition(), texProgram);
			updateTime(deltaTime);
		}
	}
}

void Scene::updateTime(int deltaTime)
{
	if (tiempoMensaje == 0) {
		if (tiempoSobrante == 0)
		{
			sceneIn->setState(SceneInteractive::GameOver);
			isPlayerDead = true;
			tiempoMensaje = 3;
			timer = 0;
			//estado = GameOver;
		}
		else if (currentTime / 1000 != timer)
		{
			--tiempoSobrante;
			timer = currentTime / 1000;
		}
	}
	else {
		if (currentTime / 1000 != timer) {
			--tiempoMensaje;
			timer = tiempoMensajeActual / 1000;
		}
	}
}

void Scene::readyMessage()
{
	sceneIn->setState(SceneInteractive::gamestate::Playing);
	estado = Playing;
	isStarting = false;
}

void Scene::updateSceneInterface(int deltaTime)
{
	sceneIn->updatePuntos(player->getScore());
	sceneIn->updatetiempoRestante(tiempoSobrante);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(camara->getPosCamara() * 0.2, 0.0f, 0.0f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	fondo->render();
	mapInteractive->render();
	player->render();
	sceneIn->render(player->getPosition());
	
	for (int i = 0; i < numGoombas; i++) goomba[i].render();
	for (int i = 0; i < numKoompas; i++)koopaTroopa[i].render();
	for (int i = 0; i < numPowerUp; i++)powerUp[i].render();
}

char Scene::getState()
{
	if (estado == Starting) return 's';
	else if (estado == Playing)  return 'a';
	else if (estado == Pause) return 'p';
	else if (estado == GameOver)  return 'o';
	else if (estado == LevelComplete) return 'l';
}

void Scene::setState(char state)
{
	if (state == 's') estado = Starting;
	else if (state == 'a')  estado = Playing;
	else if (state == 'p') estado = Pause;
	else if (state == 'o')  estado = GameOver;
	else if (state == 'l') estado = LevelComplete;
}


void Scene::initPlayer() {
	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * mapInteractive->getTileSize(), INIT_PLAYER_Y_TILES * mapInteractive->getTileSize()));
	player->setTileMap(mapInteractive);

	numPowerUp = 0;
}

void Scene::initEnemics(int nivell) {
	if (nivell == 1) {
		numGoombas = 4;
		numKoompas = 3;
		numPowerUp = 0;
		goomba = new Goomba[numGoombas];
		goomba[0].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba[0].setPosition(glm::vec2(30 * mapInteractive->getTileSize(), (9 + 0.6f) * mapInteractive->getTileSize()));
		goomba[0].setTileMapInteractive(mapInteractive);
		goomba[0].setPlayer(player);

		goomba[1].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba[1].setPosition(glm::vec2(42 * mapInteractive->getTileSize(), (9 + 0.6f) * mapInteractive->getTileSize()));
		goomba[1].setTileMapInteractive(mapInteractive);
		goomba[1].setPlayer(player);

		goomba[2].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba[2].setPosition(glm::vec2(24 * mapInteractive->getTileSize(), 5.6f * mapInteractive->getTileSize()));
		goomba[2].setTileMapInteractive(mapInteractive);
		goomba[2].setPlayer(player);

		goomba[3].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		goomba[3].setPosition(glm::vec2(24 * mapInteractive->getTileSize(), 9.6f * mapInteractive->getTileSize()));
		goomba[3].setTileMapInteractive(mapInteractive);
		goomba[3].setPlayer(player);

		koopaTroopa = new KoopaTroopa[numKoompas];
		koopaTroopa[0].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		koopaTroopa[0].setPosition(glm::vec2(160 * mapInteractive->getTileSize(), 9 * mapInteractive->getTileSize()));
		koopaTroopa[0].setTileMapInteractive(mapInteractive);
		koopaTroopa[0].setPlayer(player);

		koopaTroopa[1].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		koopaTroopa[1].setPosition(glm::vec2(20 * mapInteractive->getTileSize(), 9 * mapInteractive->getTileSize()));
		koopaTroopa[1].setTileMapInteractive(mapInteractive);
		koopaTroopa[1].setPlayer(player);

		koopaTroopa[2].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		koopaTroopa[2].setPosition(glm::vec2(10 * mapInteractive->getTileSize(), 9 * mapInteractive->getTileSize()));
		koopaTroopa[2].setTileMapInteractive(mapInteractive);
		koopaTroopa[2].setPlayer(player);

		powerUp = new PowerUp[20];
	}
}

void Scene::collisionCloscaGoomba() {
	for (int i = 0; i < numKoompas; i++) {
		if (koopaTroopa[i].isCloscaMooving()) {
			for (int j = 0; j < numGoombas; j++) {
				if (goomba[j].isAlive() && colliding(koopaTroopa[i].getPos(), goomba[j].getPos())){
					goomba[j].dies();
					player->enemyKilled();
				}
			}
		}
	}
}

bool Scene::colliding(glm::ivec2 pos1, glm::ivec2 pos2) {
	if (abs(pos1.y - pos2.y) > 24) return false;
	if (pos1.x < pos2.x && pos1.x + 24 > pos2.x) return true;
	if (pos2.x < pos1.x && pos2.x + 24 > pos1.x) return true;
	return false;
}

void Scene::updatePlayer(int deltaTime) {
	player->update(deltaTime, camara->getPosCamara());
}

void Scene::updateEnemies(int deltaTime) {
	for (int i = 0; i < numGoombas; i++) goomba[i].update(deltaTime, player->getPosition());
	for (int i = 0; i < numKoompas; i++) koopaTroopa[i].update(deltaTime, player->getPosition());
	for (int i = 0; i < numPowerUp; i++) powerUp[i].update(deltaTime, player->getPosition());
}

void Scene::updateItems(int deltaTime) {
	glm::ivec2 aux = mapInteractive->needNewBolet();
	if (aux.x != 0 && aux.y != 0) {
		powerUp[numPowerUp].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 2);
		powerUp[numPowerUp].setPosition(glm::vec2(aux.x, aux.y));
		powerUp[numPowerUp].setTileMapInteractive(mapInteractive);
		powerUp[numPowerUp].setPlayer(player);
		numPowerUp++;
	}
	aux = mapInteractive->needNewCoin();
	if (aux.x != 0 && aux.y != 0) {
		powerUp[numPowerUp].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 1);
		powerUp[numPowerUp].setPosition(glm::vec2(aux.x, aux.y));
		powerUp[numPowerUp].setTileMapInteractive(mapInteractive);
		powerUp[numPowerUp].setPlayer(player);
		player->getCoin();
		numPowerUp++;

	}
	aux = mapInteractive->needNewStar();
	if (aux.x != 0 && aux.y != 0) {
		powerUp[numPowerUp].init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram, 3);
		powerUp[numPowerUp].setPosition(glm::vec2(aux.x, aux.y));
		powerUp[numPowerUp].setTileMapInteractive(mapInteractive);
		powerUp[numPowerUp].setPlayer(player);
		numPowerUp++;
	}
}

void Scene::setSceneInterfaceState(char c) {
	if ('p' == c) sceneIn->setState(SceneInteractive::Playing);
}



