#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */


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


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, std::shared_ptr<Texture> diffuseTexture)
	: Material(shader, color), _diffuseTexture(diffuseTexture)
{
}

TextureMaterial::~TextureMaterial()
{
}

void TextureMaterial::setUniforms()
{
	Material::setUniforms();

	_diffuseTexture->bind();
	_shader->setUniform("diffuseTexture", 0);
}