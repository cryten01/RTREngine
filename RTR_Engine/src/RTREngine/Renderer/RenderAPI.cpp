#include "rtrpch.h"

#include "RTREngine/Renderer/RenderAPI.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"


namespace RTREngine {

	RenderAPI::API RenderAPI::s_API = RenderAPI::API::OpenGL;

	Scope<RenderAPI> RenderAPI::Create()
	{
		switch (s_API)
		{
		case RenderAPI::API::None:    RTR_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateScope<OpenGLRenderAPI>();
		}

		RTR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}