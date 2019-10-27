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

	void bind(); // Todo: make it compatible with texture units
};

