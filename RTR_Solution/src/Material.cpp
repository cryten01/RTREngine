#include "Material.h"


Material::Material(std::shared_ptr<Shader> shader, glm::vec3 color)
	: _shader(shader), _color(color)
{
}

Material::~Material()
{
}

Shader* Material::getShader()
{
	return _shader.get();
}

void Material::setUniforms()
{
	_shader->setUniform("diffuseColor", _color);
}