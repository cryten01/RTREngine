#pragma once
#ifdef RTR_PLATFORM_WINDOWS

extern RTREngine::App* RTREngine::CreateApp();


int main(int argc, char** argv)
{
	RTREngine::Log::Init();

	RTR_CORE_INFO("Initialized Log");
	RTR_INFO("Hello var={0}", 10);

	auto app = RTREngine::CreateApp();
	app->Run();
	delete app;
}

#endif // RTR_PLATFORM_WINDOWS
