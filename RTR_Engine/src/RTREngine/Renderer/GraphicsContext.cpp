#include "rtrpch.h"

#include "RTREngine/Renderer/Renderer.h"
#include "RTREngine/Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace RTREngine {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    RTR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		RTR_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}