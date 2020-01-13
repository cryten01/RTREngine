#include <RTR.h>

class ExampleLayer : public RTREngine::Layer
{
public:
	ExampleLayer() 
		: Layer("Example") 
	{
	};

	void OnUpdate() override 
	{
		RTR_CLIENT_INFO("ExampleLayer::Update");
	};

	void OnEvent(RTREngine::Event& event) override
	{
		RTR_CLIENT_INFO("{0}", event);
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