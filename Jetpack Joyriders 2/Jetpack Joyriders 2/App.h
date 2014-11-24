#pragma once

#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6
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
	void Update();

	float lastFrameTicks;
	float elapsed, delay, timer;

	GLuint SpriteSheetTextureID;

	vector<Entity*> Entities;
	Entity player;
	Entity Ast[40];

	Entity bullets[20];
	void shootbullet();
	bool shouldRemoveBullet(Entity bullet);
	unsigned int bulletindex;

	const Uint8* keys;
	SDL_Window* displayWindow;

	float timeLeftOver;

	void checkCollision();
	void fadeIn();
	float fadeframes;

	//float perlinValue;
	void screenShake();
	float screenShakeValue;

	float animationTime;
	float animationStart;
	float animationEnd;


};