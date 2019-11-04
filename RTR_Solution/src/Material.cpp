#include "Material.h"

/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */


Material::Material(std::shared_ptr<Shader> shader, glm::vec3 lightCoefficients, float specularCoefficient, glm::vec3 color)
	: _shader(shader), _lightCoefficients(lightCoefficients), _shininess(specularCoefficient), _color(color)
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
	_shader->setUniform("material.materialCoefficients", _lightCoefficients);
	_shader->setUniform("material.shininess", _shininess);
	_shader->setUniform("param.state", _state);
	_shader->setUniform("skybox", 0);
	_shader->setUniform("material.color", _color);
}


/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 lightCoefficients, float specularCoefficient, Texture texture)
	: Material(shader, lightCoefficients, specularCoefficient, glm::vec3(1.0f, 0.0f, 1.0f))
{
	_textures.push_back(texture);
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 lightCoefficients, float specularCoefficient, std::vector<Texture> textures)
	: Material(shader, lightCoefficients, specularCoefficient, glm::vec3(1.0f, 0.0f, 1.0f)), _textures(textures)
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