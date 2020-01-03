#include "ParticleSystem.h"

using namespace RTREngine;

/**
*	HELP
*	create 2 SSBOs		1 for read. 1 for write - see PingPonging
*	glGenBuffers		Create Bufferobject (name only, context still needs to be defined)
*	glBindBuffer		Connects GL_SHADER_STORAGE (Binding Point) with the SSBO
*	glBufferdata		Define data-structure (defines how big the buffer is and what data it contains)
*	GL_DYNAMIC_DRAW		Content of data-storage gets repeatedly declared by application.
**/
ParticleSystem::ParticleSystem(std::vector<Particle> emitters, std::shared_ptr<Material> emitterMaterial, std::shared_ptr<Shader> computeShader, std::shared_ptr<Shader> renderShader)
	: _emitterMaterial(emitterMaterial), _computeShader(computeShader), _renderShader(renderShader)
{
	// Init values
	_pingPongIndex = 0;
	_remainingToSpawn = 0;
	_particleCount = 0;


	// Create an empty set of 2 SSBOs (1x position-read/write, 1x velocity-read/write each because of PingPonging)
	glGenBuffers(1, &_ssbos[0]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbos[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ssbos[1]);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbos[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(Particle), NULL, GL_DYNAMIC_DRAW);


	// Create 2 VAOs for the renderShader not the computeShader!
	glGenVertexArrays(2, _vaos);

	for (size_t i = 0; i < 2; i++)
	{
		// Connect SSBO[i] to VAO[i];
		glBindVertexArray(_vaos[i]);

		// Bind position to attribute input location 0 in renderShader (note the offset)
		glBindBuffer(GL_ARRAY_BUFFER, _ssbos[i]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);

		// Unbind VAO
		glBindVertexArray(0);
	}


	// Add emitters to SSBOSet
	addEmittersToActiveSSBO(emitters);


	// Create atomicCounter (as buffer) and tempBuffer
	createAtomicCounter();
	createTempBuffer();
}


ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &_ssbos[0]);
	glDeleteBuffers(1, &_ssbos[1]);
	glDeleteBuffers(1, &_atomicCounterID);
	glDeleteBuffers(1, &_tempBufferID);
	glDeleteVertexArrays(2, _vaos);
}



/**
*	HELP
*	glMemoryBarrier					sync used when data is read on cpu that was written in gpu
*	GL_ATOMIC_COUNTER_BARRIER_BIT	accesses to atomic counters after the barrier will reflect writes prior to the barrier
*	glMapBufferRange				reads and writes into the AtomicCounter buffer
**/
void ParticleSystem::update(float deltaTime)
{
	// Snow mode (for testing purposes only!)
	float rangeX = rand() % 80 + (-40);
	float rangeZ = rand() % 80 + (-40);
	float dimm = 0.03f;

	// Set uniforms
	_computeShader->use();
	_computeShader->setUniform("DeltaT", deltaTime);
	_computeShader->setUniform("LastCount", _particleCount);
	_computeShader->setUniform("SpawnCount", getReadyToSpawn(deltaTime));
	_computeShader->setUniform("MaximumCount", MAX_PARTICLES);
	_computeShader->setUniform("GRAVITY", glm::vec3(0, -1, 0));
	_computeShader->setUniform("WIND", glm::vec3(rangeX * dimm, 0, rangeZ * dimm));
	_computeShader->setUniform("RandomRange", glm::vec3(rangeX, 0, rangeZ));

	// Bind to current SSBO and atomic counter
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ssbos[_pingPongIndex]);	// IN 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _ssbos[!_pingPongIndex]);	// OUT
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, _atomicCounterID);

	// Ping pong between buffers
	_pingPongIndex = !_pingPongIndex;

	// Execute computeShader with a 256 work group
	GLuint groups = _particleCount / 256 + 1;
	glDispatchCompute(groups, 1, 1);				// launches compute work groups

	// Memory barrier ensures that atomicCounter gets updated first
	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	// Bind atomic buffer and its temp buffer
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, _tempBufferID);

	// Copy atomicCounter value from the atomic buffer to the temp buffer
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

	// Read atomic counter value from temp buffer
	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	// Set particleCount = atomic counter value
	_particleCount = counterValue[0];

	// Reset atomic counter in temp buffer
	counterValue[0] = 0;

	// Stop writing to the temp buffer
	glUnmapBuffer(GL_COPY_WRITE_BUFFER);

	// Copy buffer data from the temp buffer to atomic counter buffer
	glCopyBufferSubData(GL_COPY_WRITE_BUFFER, GL_ATOMIC_COUNTER_BUFFER, 0, 0, sizeof(GLuint));

	// Memory barrier ensures that everything from computeShader is written
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

void ParticleSystem::render(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{

	// Set particle systems uniforms
	_renderShader->use();
	_renderShader->setUniform("modelViewMatrix", viewMatrix * glm::mat4(1.0f));
	_renderShader->setUniform("projectionMatrix", projMatrix);

	// Set material uniforms
	_emitterMaterial->setUniforms();

	glBindVertexArray(_vaos[_pingPongIndex]);		// bind current VAO
	glDrawArrays(GL_POINTS, 0, _particleCount);		// draw particles (points are necessary for generating quads
	glBindVertexArray(0);							// stop binding


	_renderShader->unuse();
}


/**
*	Creates an atomic counter that represents the particle count synchronized across all work groups
*
*	HELP
*	glBufferSubData		updates a subset of a buffer obect's data store (= sends data to openGL, like glBufferData)
**/
void ParticleSystem::createAtomicCounter()
{
	glGenBuffers(1, &_atomicCounterID);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterID);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

	// Set up a new atomic counter with value 0
	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
}


/**
*	Creates a temporary buffer for "move to" and "read from".
*	This buffer is only necesseray when a performance warning occurs (because of reading the atomic counter)
*
*	HELP
*	GL_COPY_WRITE_BUFFER	allows copies between buffers without disturbing the OpenGL state.
**/
void ParticleSystem::createTempBuffer()
{
	glGenBuffers(1, &_tempBufferID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, _tempBufferID);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}


/**
*	Ensures frame rate independet spawning of X particles per second.
*	This happens by accumulating the count over multiple frames.
**/
GLuint ParticleSystem::getReadyToSpawn(float deltaTime)
{
	_remainingToSpawn += SPAWN_RATE_PER_SECOND * deltaTime;		// Add new particles regardless if they are complete (can be 0.5 particles etc.)

	if (_remainingToSpawn > 1)									// Check if particles at least one full particle is ready to be spawned
	{
		GLuint completeParticles = (GLuint)_remainingToSpawn;	// Determine the amount of already complete particles by cutting decimal places

		_remainingToSpawn -= completeParticles;					// Subtract complete particles from the remaining particles to spawn

		return completeParticles;
	}

	return 0;
}


/**
*	Copies particle positions to currently active SSBO based on _pingPongIndex
**/
void ParticleSystem::addEmittersToActiveSSBO(std::vector<Particle> emitters)
{
	GLuint offsetPosition = _particleCount * sizeof(Particle);
	GLuint sizePosition = emitters.size() * sizeof(Particle);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbos[_pingPongIndex]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetPosition, sizePosition, &emitters[0]);

	_particleCount += emitters.size();	// Must be determined after setting offset and size
}


/**
*	Needed for initial particle emitters that are created on the cpu side
**/
std::vector<Particle> ParticleSystem::createStarEmitter(const unsigned int TTL)
{
	// Construct emitters and add them to the emitter vector
	std::vector<Particle> emitterData;
	Particle emitterSlot;

	glm::vec4 positionTTLs[] = {
		glm::vec4(0.0f, 0.5f, 0, TTL),
		glm::vec4(0.0f, 0.5f, 0, TTL),
		glm::vec4(0.0f, 0.5f, 0, TTL),
		glm::vec4(0.0f, 0.5f, 0, TTL),
		glm::vec4(0.0f, 0.5f, 0, TTL),
	};

	glm::vec4 velocities[] = {
		glm::vec4(0, 1, 0, 0),
		glm::vec4(1, 1, 0, 0),
		glm::vec4(-1, 1, 0, 0),
		glm::vec4(-1,-1, 0, 0),
		glm::vec4(1,-1, 0, 0),
	};


	for (size_t i = 0; i < sizeof(positionTTLs) / sizeof(glm::vec4); i++)
	{
		emitterSlot.positionsTTL = positionTTLs[i];
		emitterSlot.velocity = velocities[i];
		emitterData.push_back(emitterSlot);
	}


	return emitterData;
}

std::vector<Particle> ParticleSystem::createSnowEmitter()
{
	std::vector<Particle> emitterData;
	Particle emitterSlot;

	float height = 20;

	glm::vec4 positionTTLs[] = {
	glm::vec4(1.0f, height,-2, height),
	glm::vec4(-2.0f, height, 1, height),
	glm::vec4(3.0f, height, 4, height),
	};

	glm::vec4 velocities[] = {
		glm::vec4(0, -1, 0, 0),
		glm::vec4(0, -1, 0, 0),
		glm::vec4(0, -1, 0, 0),
	};


	for (size_t i = 0; i < sizeof(positionTTLs) / sizeof(glm::vec4); i++)
	{
		emitterSlot.positionsTTL = positionTTLs[i];
		emitterSlot.velocity = velocities[i];
		emitterData.push_back(emitterSlot);
	}

	return emitterData;
}
