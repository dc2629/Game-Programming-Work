#pragma once
#include"MatrixEntities.h"
#include"ParticleSystem.h"

#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6


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
	float elapsed, delay, actualElapsed, timer, timer2;

	GLuint SpriteSheetTextureID;

	vector<Entity*> Entities;
	vector<Entity*> floor;
	Entity player;

	int paIndex1[4];
	int numFrames;
	int currentindex;

	ParticleEmitter playerParticles;

	Entity Ast[40];
	Entity Snakes[4];

	int saIndex[4];
	int snakescurrentindex;

	float gravity_y;

	Entity bulletindicators[8];
	Entity bullets[8];
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