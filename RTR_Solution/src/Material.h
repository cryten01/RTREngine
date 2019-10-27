#pragma once

#include "Utils.h"
#include "Shader.h"
#include "Texture.h"


/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

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


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

class TextureMaterial : public Material
{
protected:
	std::shared_ptr<Texture> _diffuseTexture;

public:
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, std::shared_ptr<Texture> diffuseTexture);
	virtual ~TextureMaterial();

	virtual void setUniforms();
};

