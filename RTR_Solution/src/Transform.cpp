#include "Transform.h"


Transform::Transform(glm::mat4 modelMatrix, glm::mat4 transformMatrix)
	: _modelMatrix(modelMatrix), _transformMatrix(transformMatrix)
{
	// Defaults
	this->_rotation = glm::vec3(0.0f);
	this->_position = glm::vec3(0.0f);
	this->_scale = glm::vec3(1.0f);
}


Transform::~Transform()
{
}

void Transform::setPosition(glm::vec3 position)
{
	this->_position = position;
}

void Transform::setRotation(glm::vec3 rotation)
{
	this->_rotation = rotation;
}

void Transform::setScale(glm::vec3 scale)
{
	this->_scale = scale;
}


void Transform::setModelMatrix(glm::mat4 modelMatrix)
{
	this->_modelMatrix = modelMatrix;
}

void Transform::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->_transformMatrix = transformMatrix;
}

void Transform::setParent(std::shared_ptr<Transform> parent)
{
	this->_parent = parent;
}

void Transform::updateModelMatrix()
{
	// xyz
	_modelMatrix = glm::translate(glm::mat4(1.0f), _position);

	// yaw
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.x), glm::vec3(1, 0, 0));
	// pitch
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.y), glm::vec3(0, 1, 0));
	// roll
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_rotation.z), glm::vec3(0, 0, 1));

	// scale
	_modelMatrix = glm::scale(_modelMatrix, _scale);
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
	glm::mat4 accumModel;

	// Apply transformations
	if (_parent != nullptr)
	{
		_modelMatrix = _parent->_modelMatrix * _modelMatrix;
	}

	// Set uniforms
	shader->setUniform("modelMatrix", _modelMatrix);
	shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(_modelMatrix)))); // Fixes non uniform scaling issues, done on CPU for performance reasons
}
