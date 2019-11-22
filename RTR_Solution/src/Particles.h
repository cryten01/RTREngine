#pragma once
#include "Utils.h"

struct ParticleData
{
	std::vector<glm::vec4> positionsTTL;  // includes TimeToLive
	std::vector<glm::vec4> velocity;
};

class Particles
{
private:
	// SSBO BufferIDs (Arrays of size 2 because of PingPonging)
	GLuint _ssboPos[2];
	GLuint _ssboVel[2];	
	GLuint _vaos[2];
	GLuint _atomicCounterID;
	GLuint _tempBufferID;

	const unsigned int MAX_PARTICLES = 10000;

	void createAtomicCounter();

public:
	 Particles(ParticleData data);
	~Particles();

	static ParticleData createSnow(const unsigned int TTL);
};

