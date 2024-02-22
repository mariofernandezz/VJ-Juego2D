#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMapInteractive.h"

#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>


using namespace std;


TileMapInteractive* TileMapInteractive::createTileMapInteractive(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	TileMapInteractive* map = new TileMapInteractive(levelFile, minCoords, program);
	return map;
}


TileMapInteractive::TileMapInteractive(const string& levelFile, const glm::vec2& minCoords, ShaderProgram& program)
{
	loadLevel(levelFile, program);
	prepareArrays(minCoords, program);
	newStar = glm::ivec2(0, 0);
	newBolet = glm::ivec2(0, 0);
	newCoin = glm::ivec2(0, 0);
}

TileMapInteractive::~TileMapInteractive()
{
	if (map != NULL)
		delete map;
}


void TileMapInteractive::render() const
{
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			map[j * mapSize.x + i].render((i + 2) * tileSize,(j + 1) * tileSize);
		}
	}



	/*glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);*/
}

void TileMapInteractive::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMapInteractive::loadLevel(const string& levelFile, ShaderProgram& program)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new Bloc[mapSize.x * mapSize.y];
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			fin.get(tile);
			if (tile == ' ')
				map[j * mapSize.x + i].init(0, program, tilesheetFile);
			else
				map[j * mapSize.x + i].init((tile - int('0')), program, tilesheetFile);
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMapInteractive::prepareArrays(const glm::vec2& minCoords, ShaderProgram& program)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;

	nTiles = 0;
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			if (j * mapSize.x + i < mapSize.x * mapSize.y) {
				tile = map[j * mapSize.x + i].getType();
				if (tile != 0)
				{
					// Non-empty tile
					nTiles++;
					posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
					texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
					texCoordTile[1] = texCoordTile[0] + tileTexSize;
					//texCoordTile[0] += halfTexel;
					texCoordTile[1] -= halfTexel;
					// First triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					// Second triangle
					vertices.push_back(posTile.x); vertices.push_back(posTile.y);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
					vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
					vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
					vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
				}
			}
			else {
				int x = 3;
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMapInteractive::collisionMoveLeft(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + 4.f) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 2) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (y * mapSize.x + x < mapSize.x * mapSize.y && map[y * mapSize.x + x].getType() != 0)
			return true;
	}

	return false;
}
bool TileMapInteractive::collisionMoveRight(const glm::ivec2& pos, const glm::ivec2& size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 4) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 2) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (y * mapSize.x + x < mapSize.x * mapSize.y && map[y * mapSize.x + x].getType() != 0)
			return true;
	}

	return false;
}
bool TileMapInteractive::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const
{
	int x0, x1, y;

	x0 = (pos.x + 6) / tileSize;
	x1 = (pos.x + size.x - 6) / tileSize;
	y = (pos.y + size.y) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (y * mapSize.x + x < mapSize.x * mapSize.y && map[y * mapSize.x + x].getType() != 0)
		{
			if (*posY - tileSize * y + size.y <= 4)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}
bool TileMapInteractive::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool isSuperMario)
{
	int x0, x1, y;

	x0 = (pos.x + 6) / tileSize;
	x1 = (pos.x + size.x - 6) / tileSize;
	y = (pos.y - 1) / tileSize;
	for (int x = (x0 + x1)/2; x <= x1; x++)
	{
		if (y * mapSize.x + x < mapSize.x * mapSize.y && map[y * mapSize.x + x].getType() != 0)
		{
			if (*posY - tileSize * y - size.y <= 2)
			{
				switch (map[y * mapSize.x + x].collision(isSuperMario)) {
				case 1:
					newCoin.x = x * tileSize;
					newCoin.y = (y - 2) * tileSize;
					break;
				case 2:
					newBolet.x = x * tileSize;
					newBolet.y = (y - 2) * tileSize;
					break;
				case 3:
					newStar.x = x * tileSize;
					newStar.y = (y - 2) * tileSize;
					break;
				}
				//*posY = tileSize * (y + 1) + 2;
				return true;
			}
		}
	}
	for (int x = x0; x <= (x0 + x1) / 2; x++)
	{
		if (y * mapSize.x + x < mapSize.x * mapSize.y && map[y * mapSize.x + x].getType() != 0)
		{
			if (*posY - tileSize * y - size.y <= 2)
			{
				switch (map[y * mapSize.x + x].collision(isSuperMario)) {
				case 1:
					newCoin.x = x * tileSize;
					newCoin.y = (y - 1) * tileSize;
					break;
				case 2:
					newBolet.x = x * tileSize;
					newBolet.y = (y - 1) * tileSize;
					break;
				case 3:
					newStar.x = x * tileSize;
					newStar.y = (y - 1) * tileSize;
					break;
				}
				//*posY = tileSize * (y + 1) + 2;
				return true;
			}
		}
	}

	return false;
}

glm::ivec2 TileMapInteractive::needNewCoin() {
	glm::ivec2 aux = newCoin;
	newCoin = glm::ivec2(0, 0);
	return aux;
}
glm::ivec2 TileMapInteractive::needNewStar() {
	glm::ivec2 aux = newStar;
	newStar = glm::ivec2(0, 0);
	return aux;
}
glm::ivec2 TileMapInteractive::needNewBolet() {
	glm::ivec2 aux = newBolet;
	newBolet = glm::ivec2(0, 0);
	return aux;
}

