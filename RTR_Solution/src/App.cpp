#pragma once
#include "Utils.h"
#include "Framework.h"

using namespace RTREngine;

int main(int argc, char** argv)
{
	// Load framework
	Framework& framework = Framework::Instance();
	framework.init();

	// Load test scene
	std::shared_ptr<Window> window = framework.getWindow();
	Resources& resources = Resources::Instance();
	window->setActiveScene(resources.loadTestScene());

	//Start render loop
	framework.startRenderLoop();
	
	// Destroy framework
	framework.destroy();
}