#pragma once
#include "Utils.h"
#include "Framework.h"

using namespace RTREngine;

int main(int argc, char** argv)
{
	Framework::Instance().init();

	std::shared_ptr<Window> window = Framework::Instance().getWindow();
	window->setActiveScene(Resources::Instance().loadTestScene());

	Framework::Instance().destroy();
}