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
	void drawGeometryTest(std::shared_ptr<Shader> shader);
};

