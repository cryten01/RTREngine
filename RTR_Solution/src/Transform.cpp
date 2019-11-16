#include "Transform.h"


Transform::Transform(glm::mat4 modelMatrix, glm::mat4 transformMatrix)
	: _modelMatrix(modelMatrix), _transformMatrix(transformMatrix)
{
	// Defaults
	this->_localRot = glm::vec3(0.0f);
	this->_localPos = glm::vec3(0.0f);
	this->_localScale = glm::vec3(1.0f);
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

void Transform::setGlobalPos(glm::vec3 position)
{
	this->_globalPos = position;
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
	_modelMatrix = glm::translate(glm::mat4(1.0f), _localPos);

	// yaw
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.x), glm::vec3(1, 0, 0));
	// pitch
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.y), glm::vec3(0, 1, 0));
	// roll
	_modelMatrix = glm::rotate(_modelMatrix, glm::radians(_localRot.z), glm::vec3(0, 0, 1));

	// scale
	_modelMatrix = glm::scale(_modelMatrix, _localScale);
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
	if (_parent != nullptr)
	{
		// Hierarchical transformation
		_modelMatrix = _parent->_modelMatrix * _modelMatrix;

		// Extract globalPos
		_globalPos = glm::vec3(_modelMatrix[3][0], _modelMatrix[3][1], _modelMatrix[3][2]);

		//std::cout << _localPos.x << " " << _localPos.y << " " << _localPos.z << " " << std::endl;
	}

	// Set uniforms
	shader->setUniform("modelMatrix", _modelMatrix);
	shader->setUniform("normalMatrix", glm::mat3(glm::transpose(glm::inverse(_modelMatrix)))); // Fixes non uniform scaling issues, done on CPU for performance reasons
}
