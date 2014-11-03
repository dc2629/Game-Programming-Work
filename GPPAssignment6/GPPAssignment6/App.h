#pragma once

#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6

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
	Entity Ast[5];

	const Uint8* keys;
	SDL_Window* displayWindow;

	float timeLeftOver;

	void checkCollision();
};