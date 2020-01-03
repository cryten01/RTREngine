#include "Transform.h"

using namespace RTREngine;

Transform::Transform(glm::mat4 modelMatrix)
	: _modelMatrix(modelMatrix)
{
	// Defaults
	this->_localRot = extractRot(modelMatrix);
	this->_localPos = extractPos(modelMatrix);
	this->_localScale = extractScale(modelMatrix);
}


Transform::~Transform()
{
}

void Transform::setLocalPos(glm::vec3 position)
{
	this->_localPos = position;
}

void Transform::setLocalRot(glm::vec3 rotation)
{
	this->_localRot = rotation;
}

void Transform::setLocalScale(glm::vec3 scale)
{
	this->_localScale = scale;
}

void Transform::setModelMatrix(glm::mat4 modelMatrix)
{
	this->_modelMatrix = modelMatrix;
}

void Transform::setTransformMatrix(glm::mat4 transformMatrix)
{
	this->_transformMatrix = transformMatrix;
}

glm::vec3& Transform::getLocalPos()
{
	return this->_localPos;
}

glm::vec3 & Transform::getLocalRot()
{
	return this->_localRot;
}

glm::vec3 & Transform::getDirection()
{
	return this->_forward;
}

void Transform::setParent(std::shared_ptr<Transform> parent)
{
	this->_parent = parent;
}

glm::vec3 Transform::extractPos(glm::mat4 modelMatrix)
{
	return glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
}

glm::vec3 Transform::extractRot(glm::mat4 modelMatrix)
{
	return glm::vec3();
}

glm::vec3 Transform::extractScale(glm::mat4 modelMatrix)
{
	return glm::vec3(modelMatrix[0][0], modelMatrix[1][1], modelMatrix[2][2]);
}

void Transform::applyLocalTRS()
{
	// Translation
	_modelMatrix = glm::translate(glm::mat4(1.0f), _localPos);

	// Rotation order yzx (see openGL-tutorial.org)
	// Rotation of y (yaw)
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.y), glm::vec3(0, 1, 0));
	// Rotation of z (roll)
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.z), glm::vec3(0, 0, 1));
	// Rotation of x (pitch)
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.x), glm::vec3(1, 0, 0));

	// Scaling
	_modelMatrix = glm::scale(_modelMatrix, _localScale);
}

void Transform::applyGlobalTRS()
{
	if (_parent != nullptr)
	{
		// Hierarchical transformation (Child.world = Parent.world * Child.local)
		_modelMatrix = _parent->_modelMatrix * _modelMatrix;
	}
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
	// Set uniforms
	shader->setUniform("modelMatrix", _modelMatrix);
	shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(_modelMatrix)))); // Fixes non uniform scaling issues, done on CPU for performance reasons
}

void Transform::update()
{
	applyLocalTRS();
	applyGlobalTRS();

	// Extract direction from modelMatrix
	glm::mat3 invert = glm::mat3(_modelMatrix);
	_forward = glm::vec3(invert[0][2], invert[1][2], -invert[2][2]);
}
