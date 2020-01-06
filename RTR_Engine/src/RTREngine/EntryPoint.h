#pragma once

#ifdef RTR_PLATFORM_WINDOWS

extern RTREngine::App* RTREngine::CreateApp();

int main(int argc, char** argv)
{
	auto app = RTREngine::CreateApp();
	app->Run();
	delete app;
}

#endif // RTR_PLATFORM_WINDOWS
