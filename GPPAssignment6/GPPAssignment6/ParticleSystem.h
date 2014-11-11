#pragma once
#include"MatrixEntities.h"
#include"Color.h"

using namespace std;

class Particle {
public:
	Vector position;
	Vector velocity;
	float lifetime;
};

class ParticleEmitter {
public:
	ParticleEmitter(unsigned int particleCount);
	ParticleEmitter();
	~ParticleEmitter();


	void Update(float elapsed);
	void Render();
	Vector position;
	Vector gravity;
	float maxLifetime;

	Vector velocity;
	Vector velocityDeviation;

	vector<Particle> particles;
};