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


void Scene::render(std::shared_ptr<Shader> shader)
{
	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		object->render(shader);
	}

	_skybox->render(Resources::Instance().skyboxShader);
}
