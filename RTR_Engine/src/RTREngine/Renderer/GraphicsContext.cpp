#include "rtrpch.h"

#include "RTREngine/Renderer/GraphicsContext.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace RTREngine {

	Scope<GraphicsContext> GraphicsContext::Create(void* window)
	{
		return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
	}

}