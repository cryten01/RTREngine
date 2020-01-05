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


void Scene::setActiveSkybox(std::shared_ptr<Skybox> skybox)
{
	_skybox = skybox;
}

void Scene::setActiveCamera(std::shared_ptr<Camera> camera)
{
	_camera = camera;
}


void Scene::update(float deltaTime) 
{
	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		object->update(deltaTime);
	}
}

void Scene::setPerFrameUniforms(std::shared_ptr<Shader> shader)
{
	shader->use();

	_skybox->setUniforms(shader);
	_camera->setUniforms(shader);

	shader->unuse();
}


void Scene::render(std::shared_ptr<Shader> shader)
{
	shader->use();

	for (std::shared_ptr<SceneObject> object : _sceneObjects)
	{
		object->setUniforms(shader);
	}

	shader->unuse();

	_skybox->setUniforms(Resources::Instance().skyboxShader);
	_skybox->render(Resources::Instance().skyboxShader);
}
