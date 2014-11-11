#include"ParticleSystem.h"

float lerp1(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

void ParticleEmitter::Render(){
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> particleVertices;

	for (unsigned int i = 0; i < particles.size(); i++) {
		particleVertices.push_back(particles[i].position.x);
		particleVertices.push_back(particles[i].position.y);
	}

	glVertexPointer(2, GL_FLOAT, 0, particleVertices.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	vector<float> particleColors;
	for (unsigned int i = 0; i < particles.size(); i++) {
		float m = (particles[i].lifetime / maxLifetime);
		particleColors.push_back(lerp1(1.0f, 1.0f, m));
		particleColors.push_back(lerp1(1.0f, 1.0f, m));
		particleColors.push_back(lerp1(1.0f, 1.0f, m));
		particleColors.push_back(lerp1(1.0f, 0.0f, m));
	}
	glColorPointer(4, GL_FLOAT, 0, particleColors.data());
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_POINTS, 0, particleVertices.size() / 2);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
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
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].lifetime += elapsed;
		particles[i].position.x += particles[i].velocity.x*elapsed;
		particles[i].position.y += particles[i].velocity.y*elapsed;
	}
}