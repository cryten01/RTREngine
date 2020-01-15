#include "rtrpch.h"

#include "RTREngine/Renderer/VertexArray.h"
#include "RTREngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RTREngine {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::API::None:    RTR_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RenderAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		RTR_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

}