#include "rtrpch.h"
#include "LegacyApp.h"

using namespace RTREngine;

void RTREngine::loadLegacyEngine()
{
	// Load framework
	Framework& framework = Framework::Instance();
	framework.init();

	// Load resources
	Resources& resources = Resources::Instance();
	resources.init();

	// Load test scene into window
	std::shared_ptr<WindowLegacy> window = framework.getWindow();
	std::shared_ptr<InputLegacy> input = framework.getInput();

	SceneResources& sceneResources = SceneResources::Instance();
	sceneResources.init(input);
	window->setActiveScene(sceneResources.loadTestScene(input));

	//Start render loop
	framework.startRenderLoop();

	// Destroy framework
	framework.destroy();
}