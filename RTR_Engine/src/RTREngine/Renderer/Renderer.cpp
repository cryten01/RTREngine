#include "rtrpch.h"

#include "RTREngine/Renderer/Renderer.h"

namespace RTREngine {

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>();

	void Renderer::Init()
	{
	}

	void Renderer::Shutdown()
	{
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}
}