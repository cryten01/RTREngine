#include <RTR.h>

class Sandbox : public RTREngine::App 
{
public:
	Sandbox()
	{
	}

	~Sandbox() 
	{
	};
};

RTREngine::App* RTREngine::CreateApp()
{
	return new Sandbox();
}