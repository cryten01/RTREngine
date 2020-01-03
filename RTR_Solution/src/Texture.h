#pragma once

#include "Utils.h"

namespace RTREngine
{
	// Necessary for ASSIMP model loader
	enum TextureType
	{
		TEX_DIFFUSE,
		TEX_SPECULAR
	};

	class Texture
	{
	public:
		GLuint _ID;
		TextureType _type;
		std::string _path;

		Texture(const char* filePath, TextureType type);
		~Texture();

		void bind(unsigned int unit);
	};
}

