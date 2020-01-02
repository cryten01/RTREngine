#include "Scene.h"


Scene::Scene()
{
}

Scene::~Scene()
{
}



void Scene::setUniforms()
{
}

void Scene::render()
{
	// Render all scene objects first
	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		object->render();
	}

	// Render skybox at last
	//_skybox->render();
}
