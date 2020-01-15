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
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Render(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		
		// Set shader uniforms here

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();
}