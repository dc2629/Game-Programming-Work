#pragma once
#include"MatrixEntities.h"

class App{
public:
	App();
	~App();

	SDL_Event EVENT;

	void Init();
	bool ProcessEvents();
	void UpdateandRender();
	void FixedUpdate();
	void Render();
	void Update(float elapsed);

	float lastFrameTicks;

	GLuint SpriteSheetTextureID;

	vector<Entity*> Entities;
	Entity player;

	const Uint8* keys;
	SDL_Window* displayWindow;

	bool App::readHeader(ifstream &stream);
	bool readLayerData(ifstream &stream);
	void buildLevel();

	unsigned char** levelData;
	vector<float> levelvertexData;
	vector<float> leveltexCoordData;

	int mapWidth;
	int mapHeight;
	float TILE_SIZE;

};