#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */


Material::Material(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, glm::vec3 color)
	: _shader(shader), _reflectionConstants(reflectionConstants), _alpha(alpha), _color(color)
{
	_state = DIFFUSE;
	_alpha = 296.0f;
}

Material::~Material()
{
}

Shader* Material::getShader()
{
	return _shader.get();
}

MaterialState& Material::getState()
{
	return this->_state;
}

void Material::setUniforms()
{
	_shader->setUniform("material.light", _reflectionConstants);
	_shader->setUniform("material.alpha", _alpha);
	_shader->setUniform("param.state", _state);
	_shader->setUniform("param.illuminated", true);
	_shader->setUniform("skybox", 0);
	_shader->setUniform("material.color", _color);
}


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, Texture texture)
	: Material(shader, reflectionConstants, alpha, glm::vec3(1.0f, 0.0f, 1.0f))
{
	_textures.push_back(texture);
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, std::vector<Texture> textures)
	: Material(shader, reflectionConstants, alpha, glm::vec3(1.0f, 0.0f, 1.0f)), _textures(textures)
{
}


TextureMaterial::~TextureMaterial()
{
}


void TextureMaterial::setUniforms()
{
	// Set all non texture related uniforms
	Material::setUniforms();

	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (int i = 0; i < _textures.size(); i++)
	{
		if (_textures[i]._type == TEX_DIFFUSE) 
		{
			diffuseNr++;
			_shader->setUniform("material.texture_diffuse" + std::to_string(diffuseNr), i);
		}

		if (_textures[i]._type == TEX_SPECULAR)
		{
			specularNr++;
			_shader->setUniform("material.texture_specular" + std::to_string(specularNr), i);
		}

		_textures[i].bind(i);
	}
}