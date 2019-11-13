#include "SceneObject.h"


SceneObject::SceneObject()
{
	// Defaults
	_transform = Transform();
}


SceneObject::~SceneObject()
{
}


Transform& SceneObject::getTransform()
{
	return this->_transform;
}


void SceneObject::setShader(std::shared_ptr<Shader> shader)
{
	this->_shader = shader;
}


void SceneObject::addChild(SceneObject child)
{
	this->_children.push_back(child);
}

void SceneObject::addComponent(SceneComponent component)
{
	this->_components.push_back(component);
}


