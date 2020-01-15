#pragma once

#include "RTREngine/Renderer/RenderAPI.h"
#include "RTREngine/Renderer/VertexArray.h"
#include "RTREngine/Renderer/Shader.h"

namespace RTREngine {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene();
		static void EndScene();
		static void Render(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
		inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;
	};



	class RenderCommand
	{
	public:
		inline static void Init()
		{
			s_RenderAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RenderAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColor(const glm::vec4& color)
		{
			s_RenderAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			s_RenderAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RenderAPI->DrawIndexed(vertexArray);
		}

	private:
		static Scope<RenderAPI> s_RenderAPI;
	};
}