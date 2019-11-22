#pragma once
#include "Utils.h"
#include "Shader.h"
#include "Particles.h"


enum TestType
{
	GEOMETRYSHADER,
	PARTICLES
};


/**
* A class for testing new concepts
**/
class Test
{
public:
	GLuint VAO;
	GLuint VBO;
	unsigned int elementCount;

	 Test(TestType type);
	~Test();

	void createGeometryTest();
	void renderGeometry(std::shared_ptr<Shader> shader);

	void createParticleTest();
	void renderParticles(std::shared_ptr<Shader> shader, glm::mat4 viewMatrix, glm::mat4 projMatrix);
};

