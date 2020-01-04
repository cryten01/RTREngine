#include "Resources.h"

using namespace RTREngine;

Resources::Resources()
{
}

Resources::~Resources() 
{
}

std::shared_ptr<Scene> Resources::loadTestScene()
{
	// Load sceneObjects
	std::shared_ptr<SceneObject> camObj = std::make_shared<SceneObject>();
	camObj->addComponent(Resources::orbitCam);

	// Load scene
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();
	scene->addSceneObject(camObj);
	scene->setActiveCamera(Resources::orbitCam);
	scene->setActiveSkybox(Resources::skybox);

	return scene;
};