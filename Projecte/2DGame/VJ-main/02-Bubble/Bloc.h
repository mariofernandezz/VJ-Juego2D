#ifndef _BLOC_INCLUDE
#define _BLOC_INCLUDE


#include "Sprite.h"




class Bloc
{

public:
	void init(int blocType, ShaderProgram& shaderProgram, string tileSheetFile);
	int collision(bool isSupermario);
	int getType();
	void render(float x, float y);

private:
	int Type;
	Texture spritesheet;
	Sprite* sprite;
	void assignaAnimació();
	void creaCastell();
};


#endif // _PLAYER_INCLUDE

