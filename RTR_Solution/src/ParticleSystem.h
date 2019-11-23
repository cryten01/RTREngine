#pragma once
#include "Utils.h"
#include "Shader.h"


struct Particle					// Note: assumes identical amounts for positionsTTL and velocity
{
	glm::vec4 positionsTTL;		// Includes TTL in order to avoid padding
	glm::vec4 velocity;
};


class ParticleSystem
{
private:
	const unsigned int MAX_PARTICLES = 10000;
	const double SPAWN_RATE_PER_SECOND = 2;

	std::shared_ptr<Shader> _computeShader;
	std::shared_ptr<Shader> _renderShader;

	GLuint _ssbos[2];			// SSBO IDs are in arrays of size 2 because of PingPonging
	GLuint _vaos[2];
	GLuint _atomicCounterID;
	GLuint _tempBufferID;

	bool _pingPongIndex;		// The currently active SSBO set
	GLuint _particleCount;		// The number of total particles existent
	GLuint _readyToSpawn;		// The number of particles that are ready for emission
	double _remainingToSpawn;	// The number of particles that are not ready for emission yet


	void createAtomicCounter();
	void createTempBuffer();
	void addToParticleCount(float deltaTime);
	void addToActiveSSBOSet(std::vector<Particle> emitters);


public:
	 ParticleSystem(std::vector<Particle> emitters, std::shared_ptr<Shader> computeShader, std::shared_ptr<Shader> renderShader);
	~ParticleSystem();

	void update(float deltaTime);
	void render(glm::mat4 viewMatrix, glm::mat4 projMatrix);


	static std::vector<Particle> createEmitters(const unsigned int TTL);
	
};