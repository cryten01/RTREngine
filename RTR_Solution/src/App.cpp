#pragma once
#include "Utils.h"
#include "Resources.h"
#include "Framework.h"

using namespace RTREngine;

int main(int argc, char** argv)
{
	// Load framework
	Framework& framework = Framework::Instance();
	framework.init();

	// Load resources
	Resources& resources = Resources::Instance();
	resources.init();

	// Load test scene into window
	std::shared_ptr<Window> window = framework.getWindow();
	window->setActiveScene(resources.loadTestScene());

	//Start render loop
	framework.startRenderLoop();
	
	// Destroy framework
	framework.destroy();
}