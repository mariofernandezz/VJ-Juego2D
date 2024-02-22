#ifndef _CAMARA_INCLUDE
#define _CAMARA_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Scene.h"


// camara is a singleton (a class with a single instance) that represents our whole application


class Camara
{
public:
	Camara(glm::ivec2 playerPos, ShaderProgram &texProgram);
	void update(glm::ivec2 playerPos, ShaderProgram &texProgram);
	float getPosCamara();

private:	
	glm::mat4 projection;
	ShaderProgram *texProgram;
	float posCamara;
};


#endif // _GAME_INCLUDE


