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


class Sandbox : public RTREngine::App 
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox() 
	{
	};
};

RTREngine::App* RTREngine::CreateApp()
{
	return new Sandbox();
}