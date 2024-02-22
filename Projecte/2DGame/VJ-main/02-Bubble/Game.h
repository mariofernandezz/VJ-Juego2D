#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include "Scene.h"
#include "Menu.h"
#include "Camara.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game() {}
	
public:
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);
	
	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

	void optionSelected();


private:
	void initShaders();
	void updateRatioWindowSize(int width, int height);
	void cargaPrimerNivel();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;


	bool bPlay;                       // Continue to play game?
	Scene *scene;                      // Scene to render
	bool keys[256], specialKeys[256]; // Store key states so that 
	                                  // we can have access at any time

	Menu menu;						//Menu principal
	bool isMenuShowing;				//True = muestra menu, False = muestra juego
	bool noMenuPrincipal = false;
	int opciones;					//Opcion seleccionada del menu {Start, Help, Credits, Exit}
	int nivelActual;				//Nivel actual
	int nivelesCompletados = 0;
	int score = 0;

	Camara* camara;
};


#endif // _GAME_INCLUDE


