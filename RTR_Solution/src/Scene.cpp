#include "Scene.h"

using namespace RTREngine;

Scene::Scene()
{
}

Scene::~Scene()
{
}


void Scene::addSceneObject(std::shared_ptr<SceneObject> sceneObject)
{
	_sceneObjects.push_back(sceneObject);
}


void Scene::setActiveCamera(std::shared_ptr<Camera> camera)
{
	this->_camera = camera;
}


void Scene::setActiveSkybox(std::shared_ptr<Skybox> skybox)
{
	_skybox = skybox;
}


void Scene::update(float deltaTime) 
{
	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		object->update(deltaTime);
	}
}

void Scene::render()
{
	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		//object->render(Resources::Instance().standardShader);
	}

	_skybox->render(Resources::Instance().skyboxShader, _camera->getViewMatrix(), _camera->getProjMatrix());
}
