#pragma once

#include "Utils.h"
#include "Shader.h"
#include "Texture.h"


/**
* Base material
**/

class Material
{
protected:
	std::shared_ptr<Shader> _shader;
	glm::vec3 _color;

public:
	Material(std::shared_ptr<Shader> shader, glm::vec3 color);
	virtual ~Material();

	Shader* getShader();
	virtual void setUniforms();
};


class TextureMaterial : public Material
{
protected:
	std::vector<Texture> _textures;

public:
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, Texture texture);
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, std::vector<Texture> textures);
	virtual ~TextureMaterial();

	virtual void setUniforms();
};

