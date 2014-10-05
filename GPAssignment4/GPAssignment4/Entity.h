#pragma once
#include<SDL.h>
#include<SDL_image.h>
#include<SDL_opengl.h>
#include<iostream>
#include<string>
#include<stdio.h>
#include<vector>
#include<algorithm>

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

	bool collideRight;
	bool collideLeft;
	bool collideTop;
	bool collideBot;

	bool enableCollisions;
	bool isStatic;

	bool Entity::checkCollision(Entity A);
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