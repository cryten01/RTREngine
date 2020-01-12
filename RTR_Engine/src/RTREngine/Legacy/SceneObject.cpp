#include "rtrpch.h"
#include "SceneObject.h"

using namespace RTREngine;

SceneObject::SceneObject(glm::mat4 modelMatrix)
{
	// Defaults
	_active = true;
	_transform = std::make_shared<Transform>(modelMatrix);
	this->addComponent(_transform);
}


SceneObject::~SceneObject()
{
}

std::shared_ptr<Transform> SceneObject::getTransform()
{
	return this->_transform;
}

void SceneObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->_transform = transform;
}


void SceneObject::addChild(std::shared_ptr<SceneObject> child)
{
	// Adds child to list (owned by this)
	this->_children.push_back(child);

	// Sets parent transform (owned by this->_transform)
	child->getTransform()->setParent(this->_transform);
}

void SceneObject::addComponent(std::shared_ptr<SceneComponent> component)
{
	_components.push_back(component);
}

void SceneObject::update(float deltaTime)
{
	for (std::shared_ptr<SceneComponent> component : _components)
	{
		component->update(deltaTime);
	}

	// Update children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->update(deltaTime);
	}
}

void SceneObject::setUniforms(std::shared_ptr<Shader> shader)
{
	for (std::shared_ptr<SceneComponent> component : _components)
	{
		component->setUniforms(shader);
	}

	// setUniforms in children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->setUniforms(shader);
	}
}