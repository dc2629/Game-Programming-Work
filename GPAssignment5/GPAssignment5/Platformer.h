#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<stdio.h>
#include<vector>
#include<algorithm>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include<SDL_mixer.h>

#define MAX_ENEMIES 8
#define MAX_BULLETS 15
#define PI 3.141592

#define FIXED_TIMESTEP 0.016667
#define MAX_TIMESTEP 6

#define RANDOM_NUMBER ((float)rand()/(float)RAND_MAX)



using namespace std;


class Entity {
public:

	void Draw();

	float x;
	float y;
	int gridX;
	int gridY;
	float rotation;

	GLint textureID;
	int index;
	int spriteCountX;
	int spriteCountY;
	bool visible;
	float timeAlive;
	int health;

	float width;
	float height;

	float velocity_x;
	float velocity_y;

	float acceleration_x;
	float acceleration_y;

	float mass;
	float friction_x;
	float friction_y;

	bool collideRight;
	bool collideLeft;
	bool collideTop;
	bool collideBot;

	bool flipX;

	bool enableCollisions;
	bool isStatic;

	void resetCollisions();
	float CalculateY_Pen(Entity A);
	float CalculateX_Pen(Entity A);
	bool checkCollision(Entity A);
};

class TextEntity{
public:

	GLuint textureID;
	void DrawText();

	string text;
	float size;
	float spacing;
	float r;
	float g;
	float b;
	float a;
	float x;
	float y;
};


class DemoApp{
public:
	DemoApp();
	~DemoApp();

	SDL_Event EVENT;

	void Init();
	bool ProcessEvents();
	void UpdateandRender();
	void FixedUpdate();
	void Render();
	void GameRender();
	void MainRender();
	void EndRender();
	void Update(float elapsed);

	float lastFrameTicks;

	vector<Entity*> entities;

	GLuint SpriteSheetTextureID;

	const Uint8* keys;
	SDL_Window* displayWindow;

	float timeLeftOver = 0.0f;
	float gravity_y = -1.0f;

	Entity player;
	Entity bullet;

	enum GameState { STATE_MAIN_MENU, STATE_GAME_LEVEL, STATE_GAME_OVER };
	int State;

	unsigned char** levelData;
	//unsigned char level1Data[LEVEL_HEIGHT][LEVEL_WIDTH];

	void buildLevel();
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
	int mapWidth;
	int mapHeight;
	vector<float> levelvertexData;
	vector<float> leveltexCoordData;
	float TILE_SIZE;

	void worldToTileCoordinates(float X, float Y, int& gridX, int& gridY);
	float tiletoWorldCoordinatesx(int gridX);
	float tiletoWorldCoordinatesy(int gridY);
	bool TileCollisonX(Entity &entity);
	bool TileCollisonY(Entity &entity);
	Mix_Music *music;
	Mix_Chunk *gunsblazing;
	Mix_Chunk *mariojump;
};