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

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color)
	: Material(shader, color)
{
}

TextureMaterial::TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 color, std::shared_ptr<Texture> diffuseTex)
	: Material(shader, color), _diffuseTexture(diffuseTex)
{
}


TextureMaterial::~TextureMaterial()
{
}


void TextureMaterial::setUniforms()
{
	// Set all non texture related uniforms
	Material::setUniforms();

	// Define starting indices for all texture types
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	// Used for legacy code only!
	if (_diffuseTexture != NULL)
	{
		// Set uniform of diffuse texture
		_shader->setUniform("diffuseTexture", 0);

		// Bind diffuse texture
		_diffuseTexture->bind(0);
	}
	else 
	{
		// Loop through all textures and bind them. Also set all texture uniforms
		for (unsigned int i = 0; i < _textures.size(); i++)
		{
			// Set correct uniform by retrieving texture number (the N in diffuse_textureN)
			texType type = _textures[i]->_type;

			if (type == texture_diffuse) {
				diffuseNr++;
				_shader->setUniform("material.texture_diffuse" + std::to_string(diffuseNr), i);
			}
			else if (type == texture_specular) {
				specularNr++;
				_shader->setUniform("material.texture_specular" + std::to_string(specularNr), i);
			}

			// Bind and activate correct texture unit
			_textures[i]->bind(i);
		}
	}
}