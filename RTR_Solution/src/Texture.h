#pragma once

#include "Utils.h"

class Texture
{
private:
	GLuint _ID;
	bool _init;

public:
	 Texture(const char* texFilePath);
	~Texture();

	void bind(unsigned int unit);
};

