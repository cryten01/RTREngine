#pragma once

#include "Utils.h"

// Necessary for assimp model loader
enum texType 
{ 
	texture_diffuse, 
	texture_specular
};

class Texture
{
public:
	GLuint _ID;
	texType _type;
	std::string _path;

	 Texture(const char* texFilePath, texType type);
	~Texture();

	void bind(unsigned int unit);
};

