#pragma once
#include "Utils.h"
#include "Shader.h"

/**
* A class for testing new concepts
**/
class Testing
{
public:
	GLuint VAO;
	GLuint VBO;

	 Testing();
	~Testing();

	void createGeometryTest();
	void renderGeometry(std::shared_ptr<Shader> shader);
};

