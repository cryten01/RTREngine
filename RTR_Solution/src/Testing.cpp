#include "Testing.h"



Testing::Testing()
{
}


Testing::~Testing()
{
}

void Testing::createGeometryTest()
{
	float points[] = {
	-0.5f,  0.5f, // top-left
	 0.5f,  0.5f, // top-right
	 0.5f, -0.5f, // bottom-right
	-0.5f, -0.5f  // bottom-left
	};
}

void Testing::drawGeometryTest(std::shared_ptr<Shader> shader)
{
	shader->use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);
}
