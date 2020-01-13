#pragma once
#include "RTREngine/Core/Core.h"

#ifdef RTR_PLATFORM_WINDOWS

extern RTREngine::App* RTREngine::CreateApp();


int main(int argc, char** argv)
{
	RTREngine::Log::Init();

	RTR_CORE_INFO("Initialized Log");

	auto app = RTREngine::CreateApp();
	app->Run();
	delete app;
}

#endif
