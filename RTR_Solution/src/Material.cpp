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
	_shader->setUniform("param.type", 3);
	_shader->setUniform("skybox", 0);
	_shader->setUniform("diffuseColor", _color);
}


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, Texture texture)
	: Material(shader, color)
{
	_textures.push_back(texture);
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, std::vector<Texture> textures)
	: Material(shader, color), _textures(textures)
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