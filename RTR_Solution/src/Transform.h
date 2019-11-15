#pragma once
#include "Utils.h"
#include "Shader.h"

class Transform
{
private: 
	glm::mat4 _modelMatrix;			// permanent position in local space
	glm::mat4 _transformMatrix;

public:
	 Transform();
	~Transform();

	// Setter
	void setModelMatrix(glm::mat4 modelMatrix);
	void setTransformMatrix(glm::mat4 transformMatrix);

	// Methods
	void transform(glm::mat4 transformation);
	void resetModelMatrix();
	void render(Shader* shader);
};

