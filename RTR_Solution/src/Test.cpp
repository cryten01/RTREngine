#include "Test.h"


Test::Test(TestType type)
{
	if (type == GEOMETRYSHADER)
		createGeometryTest();
	if (type == PARTICLES)
		createParticleTest();
}


Test::~Test()
{
}

void Test::createGeometryTest()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
	
	// Set element count
	elementCount = sizeof(points) / sizeof(points[0]) / 5;

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);


	// Create VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	// Bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// Bind colors to location 2
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	

	// Unbind VAO
	glBindVertexArray(0);
}

void Test::renderGeometry(std::shared_ptr<Shader> shader)
{
	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);
	glBindVertexArray(0);
	shader->unuse();
}


void Test::createParticleTest()
{
	// Create emitter
	ParticleData emitter = Particles::createEmitters(10.0f);

	// Set element count
	elementCount = emitter.positionsTTL.size();

	// Create VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create positions VBO
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, emitter.positionsTTL.size() * sizeof(glm::vec4), emitter.positionsTTL.data(), GL_STATIC_DRAW);
	// Bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Unbind VAO
	glBindVertexArray(0);
}


void Test::renderParticles(std::shared_ptr<Shader> shader, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	shader->use();
	shader->setUniform("modelViewMatrix", glm::mat4(1.0f));
	shader->setUniform("viewProjectionMatrix", projMatrix);

	glBindVertexArray(VAO);				// bind current VAO
	glDrawArrays(GL_POINTS, 0, 4);		// draw particles (points are necessary for generating quads
	glBindVertexArray(0);
	shader->unuse();
}
