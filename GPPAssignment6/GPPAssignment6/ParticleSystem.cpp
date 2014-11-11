#include"ParticleSystem.h"

void ParticleEmitter::Render(){
	vector<float> particleVertices;

	for (int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
};

ParticleEmitter::ParticleEmitter(unsigned int particleCount){

};

ParticleEmitter::ParticleEmitter(){
	maxLifetime = 3.0f;
	for (int i = 0; i < 20; i++){
		Particle a;
		a.lifetime = RANDOM_NUMBER * 2;
		a.position.x = this->position.x;
		a.position.y = this->position.y;
		a.velocity.x = this->velocity.x;
		a.velocity.y = this->velocity.y;
		particles.push_back(a);
	}
};

ParticleEmitter::~ParticleEmitter(){

};

void ParticleEmitter::Update(float elapsed){

}