#include "Transform.h"


Transform::Transform()
{
	// Defaults
	_modelMatrix = glm::mat4(1.0f); // in origin
}


Transform::~Transform()
{
}


void Transform::setModelMatrix(glm::mat4 modelMatrix)
{
	this->_modelMatrix = modelMatrix;
}
