#pragma once

#include <RTR.h>

namespace WinterWonderland
{
	class SceneLayer : public RTREngine::Layer
	{
	public:
		SceneLayer();
		virtual ~SceneLayer() = default;

		virtual void OnUpdate(RTREngine::Time deltaTime) override;
	private:
		RTREngine::Ref<RTREngine::Shader> m_Shader;
		RTREngine::Ref<RTREngine::VertexArray> m_VertexArray;
	};
}