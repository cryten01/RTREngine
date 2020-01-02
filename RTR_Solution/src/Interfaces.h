#pragma once
#include "Shader.h"

// Guarantees that functions or variables are available
// Interaction and storing of different object types via same class

class SceneComponent
{
public:
	virtual void setUniforms(std::shared_ptr<Shader> shader) = 0;
};
