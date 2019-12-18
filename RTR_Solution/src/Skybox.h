#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Shader.h"

static const std::string& SKYBOX_ROOT_LOCATION = "../assets/textures/";

class Skybox
{
protected:
	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboIndices;
	GLuint _cubeMapID;
	unsigned int _elementCount;

	MeshData loadMeshData(float size);
	void loadTextures(std::vector<std::string>& textureFileNames);

public:
	 Skybox(float size, std::vector<std::string>& textureFileNames);
	~Skybox();

	void bindTextures(unsigned int unit = 7);
	void render(std::shared_ptr<Shader> shader, glm::mat4 viewMatrix, glm::mat4 projMatrix);
};

