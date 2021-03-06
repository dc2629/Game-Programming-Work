#pragma once
#include"MatrixEntities.h"
#include"Color.h"

using namespace std;

class Particle {
public:
	Vector position;
	Vector velocity;
	float lifetime;
	float sizeDeviation;
};

class ParticleEmitter {
public:
	ParticleEmitter(unsigned int particleCount) :maxLifetime(2.5f), startSize(0.1f), endSize(0.0f), particles(vector<Particle>(particleCount)){};
	ParticleEmitter();
	~ParticleEmitter();


	void Update(float elapsed);
	void Render();
	Vector position;
	Vector gravity;
	float maxLifetime;

	Vector velocity;
	Vector velocityDeviation;

	float startSize;
	float endSize;
	float sizeDeviation;

	vector<Particle> particles;
};