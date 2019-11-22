#include "Particles.h"



Particles::Particles(ParticleData data)
{
	// HELP
	// create 2x2 SSBOs		(2x position-read/write, 2x velocity-read/write) - see PingPonging
	// glGenBuffers			Create Bufferobject (name only, context still needs to be defined)
	// glBindBuffer			Connects GL_SHADER_STORAGE (Binding Point) with the SSBO
	// glBufferdata			Define data-structure (defines how big the buffer is and what data it contains)
	// GL_DYNAMIC_DRAW		Content of data-storage gets repeatedly declared by application.

	
	// create SSBOs (2x position-read/write, 2x velocity-read/write because of PingPonging)
	glGenBuffers(1, &_ssboPos[0]); 	// position_In
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboPos[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ssboPos[1]); 	// position_Out
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboPos[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ssboVel[0]); 	// velocity_In
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboVel[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &_ssboVel[1]); 	// velocity_Out
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboVel[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_PARTICLES * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);


	// copy particle positions to SSBO[0]
	GLuint offsetPosition = 0;
	GLuint sizePosition = data.positionsTTL.size() * sizeof(glm::vec4);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboPos[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetPosition, sizePosition, &data.positionsTTL[0]);

	// copy particle velocities to SSBO[0]
	GLuint offsetVelocity = 0;
	GLuint sizeVelocity = data.velocity.size() * sizeof(glm::vec4);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssboVel[0]);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, offsetVelocity, sizeVelocity, &data.velocity[0]);


	// create VAOs 
	glGenVertexArrays(2, _vaos);

	// connect SSBO[0] to VAO[0];
	glBindVertexArray(_vaos[0]);
	glBindBuffer(GL_ARRAY_BUFFER, _ssboPos[0]);
	// bind position to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	// connect SSBO[1] to VAO[1];
	glBindVertexArray(_vaos[1]);
	glBindBuffer(GL_ARRAY_BUFFER, _ssboPos[1]);
	// bind both position to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);



	// create atomic counter 
	createAtomicCounter();
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


/**
*	Creates an atomic counter for synchronizing work groups
**/
void Particles::createAtomicCounter()
{
	// HELP
	// GL_COPY_WRITE_BUFFER		allows copies between buffers without disturbing the OpenGL state.
	// glBufferSubData			updates a subset of a buffer obect's data store (= sends data to openGL, like glBufferData)

	glGenBuffers(1, &_atomicCounterID);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, _atomicCounterID);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);

	GLuint value = 0;
	glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(GLuint), &value);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);

	// because of a performance warning when reading atomic counter
	// create a buffer for "move to" and "read from" instead
	glGenBuffers(1, &_tempBufferID);
	glBindBuffer(GL_COPY_WRITE_BUFFER, _tempBufferID);
	glBufferData(GL_COPY_WRITE_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_READ);
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}


/**
*	Needed for initial particle emitters
**/
ParticleData Particles::createSnow(const unsigned int TTL)
{
	ParticleData data;

	data.positionsTTL = {
		glm::vec4(0, 0, 0, TTL),
		glm::vec4(2, 0, 1, TTL)
	};

	data.velocity = {
		glm::vec4(0, 0, 0, 0),
		glm::vec4(0, 0, 0, 0),
	};

	return std::move(data);
}