#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::render()
{
	for (std::shared_ptr<SceneObject> obj : _sceneObjects)
	{
		obj->renderAll();
	}
}
