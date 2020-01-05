#include "Skybox.h"

using namespace RTREngine;

Skybox::Skybox(std::shared_ptr<Camera> camera, float size, std::vector<std::string>& textureFileNames)
	: _camera(camera)
{
	// Create Mesh with material that contains cubemap textures
	MeshData data = createBoxGeometry(60.0f);
	_elementCount = data.indices.size();

	// Create VAO
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	// Create positions VBO
	glGenBuffers(1, &_vboPositions);
	glBindBuffer(GL_ARRAY_BUFFER, _vboPositions);
	glBufferData(GL_ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), GL_STATIC_DRAW);
	// Bind positions to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Create and bind indices VBO
	glGenBuffers(1, &_vboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// Loads textures of the cube map
	loadTextures(textureFileNames);
}


Skybox::~Skybox()
{
}


void Skybox::setUniforms(std::shared_ptr<Shader> shader)
{
	glm::mat4 projMatrix = _camera->getProjMatrix();
	glm::mat4 viewMatrix = _camera->getViewMatrix();


	// Remove translation part of view matrix so movement doesn't affect the skybox's position vectors
	if (true) 
	{
		viewMatrix = glm::mat4(glm::mat3(viewMatrix));
	}

	// Set uniforms
	shader->setUniform("testColor", glm::vec3(1.0, 0.0, 1.0)); // For debugging only
	shader->setUniform("skybox", CUBEMAP_UNIT);
	shader->setUniform("viewProjMatrix", projMatrix * viewMatrix);

	// Bind cube map textures
	bindTextures(CUBEMAP_UNIT);
}


void Skybox::bindTextures(unsigned int unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);
}


void Skybox::render(std::shared_ptr<Shader> shader)
{
	// Change depth function so depth test passes when values are <= 1.0 (so all objects are rendered in front of the skybox)
	glDepthFunc(GL_LEQUAL);

	shader->use();

	// Bind _vao
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Set depth test back to default
	glDepthFunc(GL_LESS);

	shader->unuse();
}


MeshData Skybox::createBoxGeometry(float size)
{
	MeshData data;

	data.positions = {
		// front
		glm::vec3(-size, -size,  size),
		glm::vec3(size, -size,  size),
		glm::vec3(size,  size,  size),
		glm::vec3(-size,  size,  size),
		// back	  
		glm::vec3(size, -size, -size),
		glm::vec3(-size, -size, -size),
		glm::vec3(-size,  size, -size),
		glm::vec3(size,  size, -size),
		// right  
		glm::vec3(size, -size,  size),
		glm::vec3(size, -size, -size),
		glm::vec3(size,  size, -size),
		glm::vec3(size,  size,  size),
		// left	  
		glm::vec3(-size, -size, -size),
		glm::vec3(-size, -size,  size),
		glm::vec3(-size,  size,  size),
		glm::vec3(-size,  size, -size),
		// top	  
		glm::vec3(-size,  size, -size),
		glm::vec3(-size,  size,  size),
		glm::vec3(size,  size,  size),
		glm::vec3(size,  size, -size),
		// bottom  
		glm::vec3(-size, -size, -size),
		glm::vec3(size, -size, -size),
		glm::vec3(size, -size,  size),
		glm::vec3(-size, -size,  size)
	};

	data.indices = {
		// front
		2, 1, 0,
		0, 3, 2,
		// back
		4, 7, 6,
		6, 5, 4,
		// right
		8, 11, 10,
		10, 9, 8,
		// left
		12, 15, 14,
		14, 13, 12,
		// top
		16, 19, 18,
		18, 17, 16,
		// bottom
		20, 23, 22,
		22, 21, 20
	};

	return std::move(data);
}

void Skybox::loadTextures(std::vector<std::string>& texturesFileNames)
{
	// Generate reference and bind cube map textures
	glGenTextures(1, &_cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		// Load cube map texture
		const std::string& filePath = SKYBOX_ROOT_LOCATION + texturesFileNames.at(i);
		unsigned char *texData = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

		// Generate cube map texture
		if (texData)
		{
			/** Send textures to GPU
			*	Possible because GL_TEXTURE_CUBE_MAP... represents an int. Following internal order:
			*	GL_TEXTURE_CUBE_MAP_POSITIVE_X 	Right
			*	GL_TEXTURE_CUBE_MAP_NEGATIVE_X 	Left
			*	GL_TEXTURE_CUBE_MAP_POSITIVE_Y 	Top
			*	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 	Bottom
			*	GL_TEXTURE_CUBE_MAP_POSITIVE_Z 	Front
			*	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 	Back
			**/
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);

			// Set the texture wrapping/filtering options (on the currently bound texture object)
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << filePath << std::endl;
		}

		// Free the image memory
		stbi_image_free(texData);
	}
}