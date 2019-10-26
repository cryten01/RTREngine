#include "Texture.h"



Texture::Texture(const char* texFilePath)
{
	// TODO: Use Soil for DDS

	// Generate reference and bind texture
	glGenTextures(1, &_ID);
	glBindTexture(GL_TEXTURE_2D, _ID);

	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//// Load and generate the texture
	//int width, height, nrChannels;
	//unsigned char *data = stbi_load(texFilePath, &width, &height, &nrChannels, 0);
	//if (data)
	//{
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//}
	//else
	//{
	//	std::cout << "Failed to load texture" << std::endl;
	//}

	//// Free the image memory
	//stbi_image_free(data);
}


Texture::~Texture()
{
}
