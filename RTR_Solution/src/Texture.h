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
private:
	GLuint _ID;
	bool _init;

public:
	texType _type;

	 Texture(const char* texFilePath, texType type);
	~Texture();

	void bind(unsigned int unit);
};

