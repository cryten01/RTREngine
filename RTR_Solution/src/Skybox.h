#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

static const std::string& SKYBOX_ROOT_LOCATION = "../assets/textures/";

class Skybox
{
protected:
	std::shared_ptr<Shader> _shader;
	std::shared_ptr<Camera> _camera;

	GLuint _vao;
	GLuint _vboPositions;
	GLuint _vboIndices;
	GLuint _cubeMapID;
	unsigned int _elementCount;

	MeshData loadMeshData(float size);
	void loadTextures(std::vector<std::string>& textureFileNames);

public:
	 Skybox(std::shared_ptr<Shader> shader, float size, std::vector<std::string>& textureFileNames);
	~Skybox();

	void bindTextures(unsigned int unit = 7);
	void setUniforms(glm::mat4 viewMatrix, glm::mat4 projMatrix);
	void render(glm::mat4 viewMatrix, glm::mat4 projMatrix);
};

