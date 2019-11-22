#pragma once
#include "Utils.h"
#include "Shader.h"

struct ParticleData						  // Note: assumes identical amounts for positionsTTL and velocity
{
	std::vector<glm::vec4> positionsTTL;  // includes TimeToLive
	std::vector<glm::vec4> velocity;
};

class Particles
{
private:
	const unsigned int MAX_PARTICLES = 10000;
	const double SPAWN_RATE_PER_SECOND = 2;

	std::shared_ptr<Shader> _computeShader;
	std::shared_ptr<Shader> _renderShader;

	GLuint _ssboPos[2];			// SSBO BufferIDs (Arrays of size 2 because of PingPonging)
	GLuint _ssboVel[2];	
	GLuint _vaos[2];
	GLuint _atomicCounterID;
	GLuint _tempBufferID;

	bool _pingPongIndex;			// The currently active SSBO set
	GLuint _particleCount;		// The number of total particles existent
	GLuint _readyToSpawn;		// The number of particles that are ready for emission
	double _remainingToSpawn;	// The number that are not ready for emission yet


	void createAtomicCounter();
	void createTempBuffer();
	void addToParticleCount(float deltaTime);
	void addToSSBOSet(ParticleData emitters);


public:
	 Particles(ParticleData emitters, std::shared_ptr<Shader> computeShader, std::shared_ptr<Shader> renderShader);
	~Particles();

	void update(float deltaTime);
	void render(glm::mat4 viewMatrix, glm::mat4 projMatrix);


	static ParticleData createEmitters(const unsigned int TTL);
	
};