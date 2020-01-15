#pragma once 

#include <RTR.h>
#include <RTREngine/Core/EntryPoint.h>

#include "SceneLayer.h"


class ClientApp : public RTREngine::App
{
public:
	ClientApp()
	{
		PushLayer(new WinterWonderland::SceneLayer());
	}

	~ClientApp()
	{
	};
};


RTREngine::App* RTREngine::CreateApp()
{
	return new ClientApp();
}