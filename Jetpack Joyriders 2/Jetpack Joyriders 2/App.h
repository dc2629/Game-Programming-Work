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
	float elapsed, delay, timer;

	GLuint SpriteSheetTextureID;

	vector<Entity*> Entities;
	vector<Entity*> floor;
	Entity player;

	int paIndex1[4];
	int numFrames;
	int currentindex;

	ParticleEmitter playerParticles;

	Entity Ast[40];

	float gravity_y;

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