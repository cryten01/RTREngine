#pragma once
#include "Utils.h"
#include "Interfaces.h"
#include "Shader.h"
#include "Material.h"

namespace RTREngine 
{
	struct Particle					// Note: assumes identical amounts for positionsTTL and velocity
	{
		glm::vec4 positionsTTL;		// Includes TTL in seconds in order to avoid padding
		glm::vec4 velocity;
	};


	class ParticleSystem
	{
	private:
		const unsigned int MAX_PARTICLES = 10000;
		const double SPAWN_RATE_PER_SECOND = 50;

		std::shared_ptr<Shader> _computeShader;
		std::shared_ptr<Shader> _renderShader;
		std::shared_ptr<Material> _emitterMaterial;

		bool _pingPongIndex;		// The currently active SSBO set
		GLuint _ssbos[2];			// Because of ping ponging two SSBOs are needed (1 Input, 1 Output). This also enures having all particles from the previous frame and the current one
		GLuint _vaos[2];
		GLuint _atomicCounterID;
		GLuint _tempBufferID;

		GLuint _particleCount;		// The number of total particles existent
		double _remainingToSpawn;	// The number of particles that are not ready for emission yet

		void createAtomicCounter();
		void createTempBuffer();
		GLuint getReadyToSpawn(float deltaTime);
		void addEmittersToActiveSSBO(std::vector<Particle> emitters);


	public:
		ParticleSystem(std::vector<Particle> emitters, std::shared_ptr<Material> emitterMaterial, std::shared_ptr<Shader> computeShader, std::shared_ptr<Shader> renderShader);
		~ParticleSystem();

		void update(float deltaTime);
		void render(glm::mat4 viewMatrix, glm::mat4 projMatrix);

		static std::vector<Particle> createStarEmitter(const unsigned int TTL);
		static std::vector<Particle> createSnowEmitter();
	};
}