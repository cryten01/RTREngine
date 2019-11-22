#include "Particles.h"



Particles::Particles(ParticleData emitters, std::shared_ptr<Shader> computeShader, std::shared_ptr<Shader> renderShader)
	: _computeShader(computeShader), _renderShader(renderShader)
{
	// HELP
	// create 2x2 SSBOs		(2x position-read/write, 2x velocity-read/write) - see PingPonging
	// glGenBuffers			Create Bufferobject (name only, context still needs to be defined)
	// glBindBuffer			Connects GL_SHADER_STORAGE (Binding Point) with the SSBO
	// glBufferdata			Define data-structure (defines how big the buffer is and what data it contains)
	// GL_DYNAMIC_DRAW		Content of data-storage gets repeatedly declared by application.


	// Init values
	_pingPongIndex = 0;
	_readyToSpawn = 0;
	_remainingToSpawn = 0;
	_particleCount = emitters.positionsTTL.size();


	// Create an empty set of 2 SSBOs (1x position-read/write, 1x velocity-read/write each because of PingPonging)
	for (size_t i = 0; i < 2; i++)
	{
		glGenBuffers(1, &_ssboPos[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboPos[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &_ssboVel[i]);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboVel[i]);
		glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
	}


	// Create 2 VAOs that each contain a position SSBO for the renderShader
	glGenVertexArrays(2, _vaos);

	for (size_t i = 0; i < 2; i++)
	{
		// Connect SSBO[i] to VAO[i];
		glBindVertexArray(_vaos[i]);
		glBindBuffer(GL_ARRAY_BUFFER, _ssboPos[i]);
		// Bind position to location 0 in renderShader
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

		// Unbind VAO
		glBindVertexArray(0);
	}


	// Add emitters to SSBOSet
	addToSSBOSet(emitters);

	createAtomicCounter();
	createTempBuffer();
}


Particles::~Particles()
{
	glDeleteBuffers(1, &_ssboPos[0]);
	glDeleteBuffers(1, &_ssboPos[1]);
	glDeleteBuffers(1, &_ssboVel[0]);
	glDeleteBuffers(1, &_ssboVel[1]);

	glDeleteBuffers(1, &_atomicCounterID);
	glDeleteBuffers(1, &_tempBufferID);

	glDeleteVertexArrays(2, _vaos);
}



void Particles::update(float deltaTime)
{
	// HELP
	// glMemoryBarrier					sync used when data is read on cpu that was written in gpu
	// GL_ATOMIC_COUNTER_BARRIER_BIT	accesses to atomic counters after the barrier will reflect writes prior to the barrier
	// glMapBufferRange					reads and writes into the AtomicCounter buffer


	// Add new particles that are readyToSpawn to particle count
	addToParticleCount(deltaTime);


	// Set uniforms
	_computeShader->use();
	_computeShader->setUniform("DeltaT", deltaTime);
	_computeShader->setUniform("MaximumCount", MAX_PARTICLES);
	_computeShader->setUniform("LastCount", _particleCount);
	_computeShader->setUniform("GRAVITY", glm::vec3(0, 0, 0));


	// Bind to current SSBO and atomic counter
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _ssboPos[_pingPongIndex]);	// IN 
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _ssboVel[_pingPongIndex]);	// IN
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, _ssboPos[!_pingPongIndex]);	// OUT
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, _ssboVel[!_pingPongIndex]);	// OUT
	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 4, _atomicCounterID);

	// Switch buffers (pingPonging)
	_pingPongIndex = !_pingPongIndex;

	// Execute computeShader with a 16x16 work group size
	GLuint groups = (_particleCount / (16 * 16)) + 1;		// determines how many particles for each workgroup (min 1)
	glDispatchCompute(groups, 1, 1);						// launches compute work groups

	// Memory barrier ensures that atomicCounter gets updated first
	glMemoryBarrier(GL_ATOMIC_COUNTER_BARRIER_BIT);

	// Bind atomic buffer and its temp buffer
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, _tempBufferID);

	// Copy atomicCounter value from the atomic buffer to the temp buffer
	glCopyBufferSubData(GL_ATOMIC_COUNTER_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0, sizeof(GLuint));

	// Receive value from temp buffer
	GLuint *counterValue = (GLuint*)glMapBufferRange(GL_COPY_WRITE_BUFFER, 0, sizeof(GLuint), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);

	// Set current particle count 
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

void Particles::render(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	// enableBlendMode()

	_renderShader->use();
	_renderShader->setUniform("modelViewMatrix", viewMatrix * glm::mat4(1.0f));
	_renderShader->setUniform("projectionMatrix", projMatrix);

	glBindVertexArray(_vaos[_pingPongIndex]);		// bind current VAO
	glDrawArrays(GL_POINTS, 0, _particleCount);		// draw particles (points are necessary for generating quads
	glBindVertexArray(0);							// stop binding

	_renderShader->unuse();

	// diableBlendMode()
}


/**
*	Creates an atomic counter that represents the particle count synchronized across all work groups
**/
void Particles::createAtomicCounter()
{
	// HELP
	// glBufferSubData		updates a subset of a buffer obect's data store (= sends data to openGL, like glBufferData)

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
*	This buffer is needed because a performance warning is raised when reading the atomic counter.
**/
void Particles::createTempBuffer()
{
	// HELP
	// GL_COPY_WRITE_BUFFER		allows copies between buffers without disturbing the OpenGL state.

	glGenBuffers(1, &_tempBufferID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, _tempBufferID);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);

	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}


/**
*	Ensures frame rate independet spawning of X particles per second.
*	This happens by accumulating the count over multiple frames.
**/
void Particles::addToParticleCount(float deltaTime)
{
	// Add new particles regardless if they are complete (can be 0.5 particles etc.)
	_remainingToSpawn += SPAWN_RATE_PER_SECOND * deltaTime;

	// Check if particles at least one full particle is ready to be spawned
	if (_remainingToSpawn > 1)
	{
		// Determine the amount of already complete particles by cutting decimal places
		GLuint completeParticles = (GLuint)_remainingToSpawn;

		// Add complete particles to spawnCount
		_particleCount += completeParticles;

		// Subtract complete particles from the remaining particles to spawn
		_remainingToSpawn -= completeParticles;
	}
}

void Particles::addToSSBOSet(ParticleData emitters)
{
	// Copy particle positions to SSBO[0]
	GLuint offsetPosition = _particleCount * sizeof(glm::vec4);
	GLuint sizePosition = emitters.positionsTTL.size() * sizeof(glm::vec4);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboPos[_pingPongIndex]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetPosition, sizePosition, &emitters.positionsTTL[0]);


	// Copy particle velocities to SSBO[0]
	GLuint offsetVelocity = _particleCount * sizeof(glm::vec4);
	GLuint sizeVelocity = emitters.velocity.size() * sizeof(glm::vec4);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboVel[_pingPongIndex]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetVelocity, sizeVelocity, &emitters.velocity[0]);
}


/**
*	Needed for initial particle emitters
**/
ParticleData Particles::createEmitters(const unsigned int TTL)
{
	ParticleData data;

	data.positionsTTL = {
		glm::vec4(-1.5f,  0.5f, 0, TTL),
		glm::vec4( 0.5f,  0.5f, 0, TTL),
		glm::vec4( 0.5f, -0.5f, 0, TTL),
		glm::vec4(-0.5f, -0.5f, 0, TTL),
	};

	data.velocity = {
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 1, 0, 0),
	};

	return std::move(data);
}