#pragma once
#include "Shader.h"

// Guarantees that functions or variables are available
// Interaction and storing of different object types via same class
namespace RTREngine
{
	class ShaderLegacy;

	class SceneComponent
	{	
	public:
		virtual void update(float deltaTime) = 0;
		virtual void setUniforms(std::shared_ptr<ShaderLegacy> shader) = 0;
	};

	class Renderable 
	{
	public:
		virtual void render(std::shared_ptr<ShaderLegacy> shader) = 0;
	};
}
