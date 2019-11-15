#include "Transform.h"


Transform::Transform(glm::mat4 modelMatrix, glm::mat4 transformMatrix)
	: _modelMatrix(modelMatrix), _transformMatrix(transformMatrix)
{
	// Defaults
	this->_modelMatrix = glm::mat4(1.0f); // in origin
}


Transform::~Transform()
{
}


void Transform::setModelMatrix(glm::mat4 modelMatrix)
{
	this->_modelMatrix = modelMatrix;
}

void Transform::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->_transformMatrix = transformMatrix;
}

void Transform::transform(glm::mat4 transformation)
{
	this->_modelMatrix = transformation * _modelMatrix;
}

void Transform::resetModelMatrix()
{
	this->_modelMatrix = glm::mat4(1);
}

void Transform::setUniforms(std::shared_ptr<Shader> shader)
{
	// Apply transformations
	glm::mat4 accumModel = _transformMatrix * _modelMatrix;

	// Set uniforms
	shader->setUniform("modelMatrix", accumModel);
	shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(accumModel)))); // Fixes non uniform scaling issues, done on CPU for performance reasons
}
