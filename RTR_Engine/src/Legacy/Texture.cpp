#include "rtrpch.h"
#include "Texture.h"

using namespace RTREngine;

Texture::Texture(const char* texFilePath, TextureType type)
	: _path(texFilePath), _type(type)
{
	// Generate reference and bind texture
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	// Load texture
	int width, height, nrChannels;
	unsigned char *texData = stbi_load(texFilePath, &width, &height, &nrChannels, STBI_rgb_alpha);

	// Generate texture
	if (texData)
	{
		// Upload texData to GPU
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the texture wrapping/filtering options (on the currently bound texture object)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);					// Magnification filter set to linear
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);						// Texture wrapping set to repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the image memory
	stbi_image_free(texData);

	// Unbinds texture
	glBindTexture(GL_TEXTURE_2D, 0);
}


void Texture::bind(unsigned int unit)
{
	// Activate proper texture unit before binding
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _ID);
}


Texture::~Texture()
{
}