#ifndef _TILE_MAP_INTERACTIVE_INCLUDE
#define _TILE_MAP_INTERACTIVE_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "Bloc.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMapInteractive
{

private:
	TileMapInteractive(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMapInteractive* createTileMapInteractive(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program);

	~TileMapInteractive();

	void render() const;
	void free();

	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const;
	bool collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool isSuperMario);

	glm::ivec2 needNewCoin();
	glm::ivec2 needNewStar();
	glm::ivec2 needNewBolet();

	
	//void setPlayer(Player &player);

private:
	bool loadLevel(const string& levelFile, ShaderProgram& program);
	void prepareArrays(const glm::vec2& minCoords, ShaderProgram& program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	int nTiles;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	glm::ivec2 newStar, newBolet, newCoin;
	Bloc* map;
	//Player* player;
};


#endif // _TILE_MAP_INCLUDE


