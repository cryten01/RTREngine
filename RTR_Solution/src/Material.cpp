#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */


Material::Material(std::shared_ptr<Shader> shader, glm::vec3 color, glm::vec3 materialCoefficients, float specularCoefficient)
	: _shader(shader), _color(color), _materialCoefficients(materialCoefficients), _shininess(specularCoefficient)
{
	// Initial values
	_state = DIFFUSE;
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
	_shader->setUniform("material.materialCoefficients", _materialCoefficients);
	_shader->setUniform("material.shininess", _shininess);
	_shader->setUniform("param.state", _state);
	_shader->setUniform("skybox", 0);
	_shader->setUniform("diffuseColor", _color);
}


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, glm::vec3 materialCoefficients, float specularCoefficient, Texture texture)
	: Material(shader, color, materialCoefficients, specularCoefficient)
{
	_textures.push_back(texture);
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, glm::vec3 materialCoefficients, float specularCoefficient, std::vector<Texture> textures)
	: Material(shader, color, materialCoefficients, specularCoefficient), _textures(textures)
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