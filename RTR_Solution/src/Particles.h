#pragma once
#include "Utils.h"
#include "Shader.h"

struct ParticleData
{
	std::vector<glm::vec4> positionsTTL;  // includes TimeToLive
	std::vector<glm::vec4> velocity;
};

class Particles
{
private:
	const unsigned int MAX_PARTICLES = 10000;
	const double SPAWN_RATE_PER_SECOND = 2;

	GLuint _ssboPos[2];		// SSBO BufferIDs (Arrays of size 2 because of PingPonging)
	GLuint _ssboVel[2];	
	GLuint _vaos[2];
	GLuint _atomicCounterID;
	GLuint _tempBufferID;

	bool _bufferIndex;
	GLuint _particleCount;		// The number of total particles existend
	GLuint _readyToSpawn;
	double _remainingToSpawn;	// The number of new particles per frame
	std::shared_ptr<Shader> _computeShader;

	void createAtomicCounter();
	void createTempBuffer();

	void calcReadyAndRemainingToSpawn(float deltaTime);


public:
	 Particles(ParticleData data, std::shared_ptr<Shader> computeShader);
	~Particles();

	void update(float deltaTime);


	static ParticleData emit(const unsigned int TTL);
	
};