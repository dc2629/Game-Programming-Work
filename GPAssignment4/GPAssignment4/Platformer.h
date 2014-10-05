#pragma once
#include"Entity.h"

class DemoApp{
public:
	DemoApp();
	~DemoApp();

	SDL_Event EVENT;

	void Setup();
	bool ProcessEvents();
	void UpdateandRender();
	void FixedUpdate();
	void Render();
	void GameRender();
	void MainRender();
	void EndRender();

	vector<Entity> enemies;
	vector<Entity> bullets;
	vector<Entity> wall;
	Entity player;
	Entity genEnemy;
	GLuint SpriteSheetTextureID;

	const Uint8* keys;
	SDL_Window* displayWindow;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int State;
};


