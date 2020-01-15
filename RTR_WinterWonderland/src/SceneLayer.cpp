#include "SceneLayer.h"
#include "Keymappings.h"

namespace WinterWonderland {
	
	SceneLayer::SceneLayer()
		: RTREngine::Layer("WinterWonderland")
	{
	};


	void SceneLayer::OnUpdate(RTREngine::Time deltaTime)
	{
		WinterWonderland::TriggerKeyCallback();

		// Render
		{
			RTREngine::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RTREngine::RenderCommand::Clear();
		}
	};
}