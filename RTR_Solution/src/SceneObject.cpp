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

std::shared_ptr<Shader> SceneObject::getShader()
{
	return this->_shader;
}

std::shared_ptr<Mesh> SceneObject::getMeshAt(int index)
{
	return this->_meshes.at(index);
}


void SceneObject::setLight(std::shared_ptr<SpotLight> pointLight)
{
	this->_light = pointLight;
}

void SceneObject::setShader(std::shared_ptr<Shader> shader)
{
	this->_shader = shader;
}

void SceneObject::setTransform(std::shared_ptr<Transform> transform)
{
	this->_transform = transform;
}

void SceneObject::addMesh(std::shared_ptr<Mesh> mesh)
{
	this->_meshes.push_back(mesh);
}


void SceneObject::addChild(std::shared_ptr<SceneObject> child)
{
	// Adds child to list (owned by this)
	this->_children.push_back(child);

	// Sets parent transform (owned by this->_transform)
	child->getTransform()->setParent(this->_transform);
}

void SceneObject::update()
{
	this->_transform->updateModelMatrix();

	if (_light != nullptr) 
	{
		// Update light position
		_light->position = _transform->getLocalPos();

		// TODO: Update light direction here
	}
}

void SceneObject::updateAll()
{
	update();

	// Update children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->updateAll();
	}
}

void SceneObject::render()
{
	// Set transform uniforms
	this->_transform->setUniforms(_shader);

	// Render each mesh of this sceneObject
	for (auto mesh : _meshes)
	{
		mesh->render();
	}
}

void SceneObject::renderAll()
{
	this->render();

	// Render children
	for (size_t i = 0; i < _children.size(); i++)
	{
		_children.at(i)->renderAll();
	}
}


