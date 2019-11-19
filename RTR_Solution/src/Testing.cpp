#include "Testing.h"



Testing::Testing()
{
}


Testing::~Testing()
{
}

void Testing::createGeometryTest()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};
	
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

void Testing::drawGeometryTest(std::shared_ptr<Shader> shader)
{
	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);
}
