#pragma once
#include "Shader.h"

// Guarantees that functions or variables are available
// Interaction and storing of different object types via same class
namespace RTREngine
{
	class SceneComponent
	{	
	public:
		virtual void update(float deltaTime) = 0;
		virtual void render(std::shared_ptr<Shader> shader) = 0;
	};
}
