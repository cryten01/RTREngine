#include "Skybox.h"


Skybox::Skybox(float size, const char* textures[])
{
	// Create Mesh with material that contains cubemap textures
	GeometryData data = loadGeometry(60.0f);
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
	loadTextures(textures);
}


Skybox::~Skybox()
{
}

void Skybox::render(std::shared_ptr<Shader> shader, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	// Remove translation part of view matrix so movement doesn't affect the skybox's position vectors
	viewMatrix = glm::mat4(glm::mat3(viewMatrix));


	// Change depth function so depth test passes when values are <= 1.0 (so all objects are rendered in front of the skybox)
	glDepthFunc(GL_LEQUAL);

	// Select shader
	shader->use();

	// Set shader uniforms (view projection matrix)
	shader->setUniform("viewProjMatrix", projMatrix * viewMatrix);

	// Bind cube map textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texturesID);

	// Bind _vao
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Set depth test back to default
	glDepthFunc(GL_LESS);
}


GeometryData Skybox::loadGeometry(float size)
{
	GeometryData data;

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

void Skybox::loadTextures(const char * textures[])
{
	// Generate reference and bind cube map textures
	glGenTextures(1, &_texturesID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _texturesID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < 6; i++)
	{
		// Load cube map texture
		unsigned char *texData = stbi_load(textures[i], &width, &height, &nrChannels, 0);

		// Generate cube map texture
		if (texData)
		{
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
			std::cout << "Cubemap texture failed to load at path: " << textures[i] << std::endl;
		}

		// Free the image memory
		stbi_image_free(texData);
	}
}
