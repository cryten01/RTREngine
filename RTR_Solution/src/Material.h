#pragma once

#include "Utils.h"
#include "Shader.h"

class Material
{
private:
	std::shared_ptr<Shader> _shader;
	glm::vec3 _color;

public:
	Material(std::shared_ptr<Shader> shader, glm::vec3 color);
	virtual ~Material();

	Shader* getShader();
	virtual void setUniforms();
};

