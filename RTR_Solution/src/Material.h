#pragma once

#include "Utils.h"
#include "Shader.h"
#include "Texture.h"



enum MaterialState
{
	REFLECTIVE = 0,
	REFRACTIVE = 1,
	TEXTURE = 2,
	DIFFUSE = 3		// Default state
};



class Material
{
protected:
	std::shared_ptr<Shader> _shader;

	MaterialState _state;
	glm::vec3 _color;
	glm::vec3 _reflectionConstants; // x = ambient, y = diffuse, z = specular
	float _alpha;

public:
	Material(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, glm::vec3 color);
	virtual ~Material();

	Shader* getShader();
	MaterialState& getState();
	void setState(MaterialState state);

	virtual void setUniforms();
};



class TextureMaterial : public Material
{
protected:
	std::vector<Texture> _textures;

public:
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, Texture texture);
	TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, std::vector<Texture> textures);
	virtual ~TextureMaterial();

	virtual void setUniforms();
};

