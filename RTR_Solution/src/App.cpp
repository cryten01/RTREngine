#pragma once
#include "Utils.h"
#include "Framework.h"

using namespace RTREngine;

int main(int argc, char** argv)
{
	Framework::Instance().init();

	// Load test scene here
	std::shared_ptr<Window> window = Framework::Instance().getWindow();
	window->setActiveScene(Resources::Instance().loadTestScene());

	Framework::Instance().startRenderLoop();
	Framework::Instance().destroy();
}