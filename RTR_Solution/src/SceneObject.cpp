#include "SceneObject.h"


SceneObject::SceneObject(std::shared_ptr<Shader> shader)
	: _shader(shader)
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


