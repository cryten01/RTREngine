#pragma once

#include "Utils.h"
#include "Shader.h"
#include "Texture.h"
#include "Interfaces.h"

namespace RTREngine
{
	enum MaterialType
	{
		TEXTURE = 0,
		DIFFUSE = 1		// Default state
	};


	class Material : public Uniformable
	{
	protected:
		std::shared_ptr<Shader> _shader;

		MaterialType _type;
		glm::vec3 _color;
		glm::vec3 _reflectionConstants; // x = ambient, y = diffuse, z = specular
		float _alpha;
		bool _isRefractive;
		bool _isReflective;

	public:
		Material(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, glm::vec3 color);
		virtual ~Material();

		Shader* getShader();
		MaterialType& getType();
		void setType(MaterialType state);
		void setIsRefractive(bool state);
		void setIsReflective(bool state);

		void setUniforms(std::shared_ptr<Shader> shader) override;
	};



	class TextureMaterial : public Material, Uniformable
	{
	protected:
		std::vector<std::shared_ptr<Texture>> _textures;

	public:
		TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, std::shared_ptr<Texture> texture);
		TextureMaterial(std::shared_ptr<Shader> shader, glm::vec3 reflectionConstants, float alpha, std::vector<std::shared_ptr<Texture>> textures);
		virtual ~TextureMaterial();

		void setUniforms(std::shared_ptr<Shader> _shader) override;
	};
}