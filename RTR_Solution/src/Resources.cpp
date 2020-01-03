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
	std::shared_ptr<Scene> scene = std::make_shared<Scene>();

	scene->setSkybox(Resources::skybox);

	return scene;
};