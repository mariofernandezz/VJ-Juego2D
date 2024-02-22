#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "TileMapInteractive.h"
#include "Player.h"
#include "Camara.h"
#include "Goomba.h"
#include "KoopaTroopa.h"
#include "PowerUP.h"
#include "SceneInteractive.h"



// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init(ShaderProgram& shaderProgram, int nivel, int puntos);
	void update(int deltaTime);
	void render();
	char getState();
	void setState(char state);
	int getPlayerScore() { return player->getScore(); };
	void initShaders();
	void updateTime(int deltaTime);

	int getLiveScore() { return score; };

	bool isMarioEstrella() { return isMarioStar; };
	void setMarioEstrella(bool isEstrella) { isMarioStar = isEstrella; player->setEstrella(isEstrella); };
	bool isMarioBros() { return bMarioBros; };
	void setMarioBros(bool isBros) { bMarioBros = isBros; player->setBros(isBros); };
	enum gamestate { Starting, Playing, Pause, GameOver, LevelComplete };
	// Estado de la partida
	void setSceneInterfaceState(char c);

	gamestate estado;


private:
	void initPlayer();
	void initEnemics(int nivell);
	void collisionCloscaGoomba();
	bool colliding(glm::ivec2 pos1, glm::ivec2 pos2);
	void updatePlayer(int deltaTime);
	void updateEnemies(int deltaTime);
	void updateItems(int deltaTime);


	void readyMessage();
	void updateSceneInterface(int deltaTime);



private:
	TileMap *fondo;
	TileMapInteractive* mapInteractive;
	SceneInteractive* sceneIn;

	Player *player;
	Goomba *goomba;
	KoopaTroopa *koopaTroopa;
	PowerUp* powerUp;
	Camara *camara;
	ShaderProgram texProgram;
	int currentTime;
	int numGoombas, numKoompas, numPowerUp;

	glm::mat4 projection;
	float posCamara;

	bool isPlayerDead;
	bool isMarioStar;
	bool bMarioBros;

	bool isStarting;
	int levelActual;

	int tiempoActual = 0;
	int tiempoSobrante = 60;

	int tiempoMensaje = 0;
	int tiempoMensajeActual = 0;

	int score = 0;
	int timer, timerD = 0;
	int timerMuerte = 25;

	bool done = false;

};


#endif // _SCENE_INCLUDE

