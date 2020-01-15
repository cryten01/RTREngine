#include "rtrpch.h"
#include "Material.h"

using namespace RTREngine;

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

Material::Material(std::shared_ptr<ShaderLegacy> shader, glm::vec3 reflectionConstants, float alpha, glm::vec3 color)
	: _shader(shader), _reflectionConstants(reflectionConstants), _alpha(alpha), _color(color)
{
	_type = DIFFUSE;
	_isReflective = false;
	_isRefractive = false;
	_alpha = 296.0f;
}

Material::~Material()
{
}

ShaderLegacy* Material::getShader()
{
	return _shader.get();
}

MaterialType& Material::getType()
{
	return this->_type;
}

void Material::setType(MaterialType state)
{
	this->_type = state;
}

void Material::setIsRefractive(bool state)
{
	this->_isRefractive = state;
}

void Material::setIsReflective(bool state)
{
	this->_isReflective = state;
}

void Material::setUniforms(std::shared_ptr<ShaderLegacy> _shader)
{
	_shader->setUniform("material.type", _type);
	_shader->setUniform("material.light", _reflectionConstants);
	_shader->setUniform("material.color", _color);
	_shader->setUniform("material.alpha", _alpha);
	_shader->setUniform("material.isRefractive", _isRefractive);
	_shader->setUniform("material.isReflective", _isReflective);
}


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<ShaderLegacy> shader, glm::vec3 reflectionConstants, float alpha, std::shared_ptr<Texture> texture)
	: Material(shader, reflectionConstants, alpha, glm::vec3(1.0f, 0.0f, 1.0f))
{
	_type = TEXTURE;

	_textures.push_back(texture);
}

TextureMaterial::TextureMaterial(std::shared_ptr<ShaderLegacy> shader, glm::vec3 reflectionConstants, float alpha, std::vector<std::shared_ptr<Texture>> textures)
	: Material(shader, reflectionConstants, alpha, glm::vec3(1.0f, 0.0f, 1.0f)), _textures(textures)
{
	_type = TEXTURE;
}


TextureMaterial::~TextureMaterial()
{
}


void TextureMaterial::setUniforms(std::shared_ptr<ShaderLegacy> _shader)
{
	// Set all non texture related uniforms
	Material::setUniforms(_shader);

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (int i = 0; i < _textures.size(); i++)
	{
		if (_textures[i]->_type == TEX_DIFFUSE) 
		{
			diffuseNr++;
			_shader->setUniform("material.texture_diffuse" + std::to_string(diffuseNr), i);
		}

		if (_textures[i]->_type == TEX_SPECULAR)
		{
			specularNr++;
			_shader->setUniform("material.texture_specular" + std::to_string(specularNr), i);
		}

		_textures[i]->bind(i);
	}
}