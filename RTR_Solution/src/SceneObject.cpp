#include "SceneObject.h"

SceneObject::SceneObject(glm::mat4 modelMatrix)
{
	// Defaults
	_active = true;
	_transform = std::make_shared<Transform>(modelMatrix);
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
	if (_active) 
	{
		//_transform->update();

		//if (_light != nullptr) 
		//{
		//	//std::cout << _transform->getLocalPos().x << " " << _transform->getLocalPos().y << " " << _transform->getLocalPos().z << " " << std::endl;

		//	// Update light position
		//	_light->position = _transform->getLocalPos();

		//	// Update light direction
		//	//_light->direction = _transform->getDirection();
		//}
	}

	// Update children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->update(deltaTime);
	}
}


void SceneObject::render(std::shared_ptr<Shader> shader)
{
	if (_active)
	{
		//for (std::shared_ptr<SceneComponent> component : _components) 
		//{
		//	component->setUniforms(shader);
		//}

		//// Set transform uniforms
		//this->_transform->setUniforms(_shader);

		//// Render each mesh of this sceneObject
		//for (auto mesh : _meshes)
		//{
		//	mesh->render();
		//}
	}

	// Render children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->render(shader);
	}
}