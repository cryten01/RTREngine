#pragma once
#include "Utils.h"
#include "SceneComponent.h"

class Transform : public SceneComponent
{
private: 
	glm::mat4 _modelMatrix; // permanent position in local space

public:
	 Transform();
	~Transform();

	void setModelMatrix(glm::mat4 modelMatrix);
};

