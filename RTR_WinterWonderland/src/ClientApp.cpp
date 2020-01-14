#include <RTR.h>

class ExampleLayer : public RTREngine::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	};

	void OnUpdate(RTREngine::Time deltaTime) override
	{
		//RTR_CLIENT_INFO("ExampleLayer::Update");
		RTR_CLIENT_TRACE("Delta time: {0} ({1}ms)", deltaTime.GetSeconds(), deltaTime.GetMilliseconds());
	};

	void OnEvent(RTREngine::Event& event) override
	{
		//RTR_CLIENT_INFO("{0}", event);
	};
};


class WinterWonderland : public RTREngine::App
{
public:
	WinterWonderland()
	{
		PushLayer(new ExampleLayer());
	}

	~WinterWonderland()
	{
	};
};

RTREngine::App* RTREngine::CreateApp()
{
	return new WinterWonderland();
}