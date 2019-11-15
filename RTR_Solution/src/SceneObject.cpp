#include "SceneObject.h"


SceneObject::SceneObject(std::shared_ptr<Shader> shader, glm::mat4 transformMatrix)
	: _shader(shader)
{
	// Defaults
	_transform = std::make_shared<Transform>(glm::mat4(1.0f), transformMatrix);
}


SceneObject::~SceneObject()
{
}


std::shared_ptr<Transform> SceneObject::getTransform()
{
	return this->_transform;
}

std::shared_ptr<Mesh> SceneObject::getMesh()
{
	return this->_mesh;
}


void SceneObject::setShader(std::shared_ptr<Shader> shader)
{
	this->_shader = shader;
}

void SceneObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->_transform = transform;
}

void SceneObject::setMesh(std::shared_ptr<Mesh> mesh)
{
	this->_mesh = mesh;
}


void SceneObject::addChild(SceneObject child)
{
	this->_children.push_back(child);
}

void SceneObject::render()
{
	// Set transform uniforms
	getTransform()->setUniforms(_shader);

	// Render mesh
	getMesh()->render();
}


