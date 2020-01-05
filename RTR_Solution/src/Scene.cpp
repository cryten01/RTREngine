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

void Scene::addRenderable(std::shared_ptr<Renderable> renderObject)
{
	_renderObjects.push_back(renderObject);
}

void Scene::addUniformable(std::shared_ptr<Uniformable> uniformObject)
{
	_uniformObjects.push_back(uniformObject);
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

void Scene::setUniforms(std::shared_ptr<Shader> shader)
{
	shader->use();

	for (std::shared_ptr<Uniformable> object : _uniformObjects)
	{
		object->setUniforms(shader);
	}

	shader->unuse();

	Resources::Instance().skyboxShader->use();

	_skybox->setUniforms(Resources::Instance().skyboxShader);

	Resources::Instance().skyboxShader->unuse();
}


void Scene::render(std::shared_ptr<Shader> shader)
{
	shader->use();

	for (std::shared_ptr<Renderable> object : _renderObjects)
	{
		object->render(shader);
	}

	shader->unuse();

	_skybox->render(Resources::Instance().skyboxShader);
}
