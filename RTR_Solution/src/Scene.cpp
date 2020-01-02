#include "Scene.h"


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

void Scene::setSkybox(std::shared_ptr<Skybox> skybox)
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
		//object->render(Resources::General::standardShader);
	}
}
