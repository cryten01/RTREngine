#include "Texture.h"



Texture::Texture(const char* texFilePath)
{
	// Generate reference and bind texture
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture
	int width, height;
	unsigned char *texData = SOIL_load_image(texFilePath, &width, &height, 0, SOIL_LOAD_RGBA);

	// Generate texture
	if (texData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the image memory
	SOIL_free_image_data(texData);
}


Texture::~Texture()
{
}
