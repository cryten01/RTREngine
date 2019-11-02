#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Shader.h"

class Skybox
{
protected:
	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboIndices;
	GLuint _texturesID;
	unsigned int _elementCount;

	GeometryData loadGeometry(float size);
	void loadTextures(const char* textures[]);

public:
	 Skybox(float size, const char* textures[]);
	~Skybox();

	void render(std::shared_ptr<Shader> shader, glm::mat4 viewMatrix, glm::mat4 projMatrix);
};

